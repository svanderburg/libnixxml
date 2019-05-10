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
        CFLAGS = "-Wall -ansi -pedantic";
        buildInputs = [ doxygen pkgconfig libxml2 libxslt getopt glib ];
        configureFlags = [ "--with-glib" ];

        preDist = ''
          make -C src apidox
          mkdir -p $out/share/doc/libnixxml
          cp -av apidox $out/share/doc/libnixxml
          echo "doc api $out/share/doc/libnixxml/apidox/html" >> $out/nix-support/hydra-build-products
        '';
      };

    build = {
      basic = pkgs.lib.genAttrs systems (system:
        with import nixpkgs { inherit system; };

        releaseTools.nixBuild {
          name = "libnixxml";
          src = tarball;
          configureFlags = [ "--with-gd" ];
          CFLAGS = "-Wall -ansi -pedantic";
          buildInputs = [ pkgconfig libxml2 gd.dev getopt libxslt nix ];
          doCheck = true;
        });

      glib = pkgs.lib.genAttrs systems (system:
        with import nixpkgs { inherit system; };

        releaseTools.nixBuild {
          name = "libnixxml";
          src = tarball;
          configureFlags = [ "--with-gd" "--with-glib" ];
          CFLAGS = "-Wall";
          buildInputs = [ pkgconfig libxml2 gd.dev glib getopt libxslt nix ];
          doCheck = true;
        });
    };
  };
in
jobs
