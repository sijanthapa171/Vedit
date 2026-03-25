{
  description = "Vedit - A Vim-like C Text Editor";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      pkgsFor = system: import nixpkgs { inherit system; };
    in
    {
      devShells = forAllSystems (system:
        let
          pkgs = pkgsFor system;
        in
        {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              gcc
              gdb
              gnumake
              pkg-config
            ];

            shellHook = ''
              echo "Vedit Development Environment Loaded"
              echo "Available tools: gcc, gdb, make, pkg-config"
            '';
          };
        }
      );
    };
}
