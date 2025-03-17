with import <nixpkgs> { };

stdenv.mkDerivation {
  name = "sdl-sample";
  src = ".";
  buildInputs = [
    gcc
    clang
    pkgs.sdl3
  ];
  buildPhase = "gcc -o main main.c";

  installPhase = ''
    mkdir -p $out/bin
    cp main $out/bin/
  '';
}
