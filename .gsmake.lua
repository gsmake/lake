return {

    -- the package's universal name
    name    = "github.com/gsmake/lake",

    -- package domain
    domain  = {"task", "lua", "native"},

    -- package's import scetion
    imports = {
        { name = "github.com/gsmake/clang", domain = "task" ,version = "release/v1.0" }
    },

    tasks = {

        {
            name = "resources",
            description = "prepare resources"
        },

        {
            name = "list",
            description = " list all task of this package defined and imports",
            task = function()
                print("-----------")
            end
        },
    }
}
