{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem [
      flake-utils.lib.system.x86_64-linux
      flake-utils.lib.system.riscv64-linux
    ]
      (system:
        let
          pkgs = import nixpkgs { inherit system; };
          cross = pkgs.pkgsCross.riscv64;
          isCross = system != flake-utils.lib.system.riscv64-linux;
          riscv_pkgs = if isCross then cross else pkgs;
          llvm_release = {
            version = "19.0.0-git";
            rev = "44af53b22aaa1fe382b22329bbc7e4610ecbacc8";
            rev-version = "19.0.0-unstable-2024-03-28";
            sha256 = "sha256-1vSe2HtMjId3bVIHfzDOgIiI5uwelvP2Pky2TpTvUoI=";
          };
          riscv_llvm =
            if isCross
            then
              cross.buildPackages.llvmPackages_git.override
                {
                  gitRelease = llvm_release;
                  officialRelease = null;
                }
            else
              pkgs.llvmPackages_git.override
                {
                  gitRelease = llvm_release;
                  officialRelease = null;
                };
        in
        {
          formatter = pkgs.nixpkgs-fmt;
          devShell = pkgs.mkShell rec {
            buildInputs = [
              riscv_llvm.clang
              riscv_llvm.bintools
              riscv_pkgs.gtest
              riscv_pkgs.libhwy
              riscv_pkgs.glibc.static
              pkgs.qemu
              pkgs.clang
              pkgs.libhwy
              pkgs.clang-tools_17
              pkgs.cpplint
              pkgs.cmake
              pkgs.ninja
              pkgs.cmake-format
              (pkgs.python311.withPackages (ps: with ps; [ pytest ]))
            ];
          };
        }
      );
}
