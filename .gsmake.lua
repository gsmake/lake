name "github.com/gsmake/lake" -- package name

--version "master" -- package version

plugin "github.com/gsmake/clang"

plugin "github.com/gsmake/lua"



lua = {

    srcDirs         = { "lib" };
}

clang = {
    modules = {
        lemon = {
            type    = "static";

            srcDir = "src/lib/lemon";
        };
    }
}