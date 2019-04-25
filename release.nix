{ nixpkgs ? <nixpkgs>
, systems ? [ "i686-linux" "x86_64-linux" ]
, libnixxml ? { outPath = ./.; rev = 1234; }
, officialRelease ? false
}:

let
  pkgs = import nixpkgs {};

  jobs = rec {
    tarball =
      with pkgs;

      releaseTools.sourceTarball {
        name = "libnixxml-tarball";
        version = builtins.readFile ./version;
        src = libnixxml;
        inherit officialRelease;
        CFLAGS = "-Wall";
        buildInputs = [ doxygen pkgconfig libxml2 gd.dev getopt libxslt ];

        preDist = ''
          make -C src apidox
          mkdir -p $out/share/doc/libnixxml
          cp -av apidox $out/share/doc/libnixxml
          echo "doc api $out/share/doc/libnixxml/apidox/html" >> $out/nix-support/hydra-build-products
        '';
      };

    build =
      pkgs.lib.genAttrs systems (system:
        with import nixpkgs { inherit system; };

        releaseTools.nixBuild {
          name = "libnixxml";
          src = tarball;
          configureFlags = [ "--with-gd" ];
          CFLAGS = "-Wall -ggdb";
          buildInputs = [ pkgconfig libxml2 gd.dev getopt libxslt ];
          doCheck = true;
          dontStrip = true;
        });
  };
in
jobs
