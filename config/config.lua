return {
    SCM = {
        default = "git",
        default_version = "snapshot",
    },

    sites = {
        ["github.com"] = function(url)
            local pattern =  [[^(github%.com/[A-Za-z0-9_%.%-]+/[A-Za-z0-9_%.%-]+)/?$]]

            local ok = false

            local newurl = url:gsub(pattern,function (prefix)
                ok = true
                return string.format("https://%s.git",prefix)
            end)

            return ok,"git",newurl
        end,
        ["bitbucket.org"] = function(url)
            local pattern = [[^(?P<root>bitbucket\.org/[A-Za-z0-9_.\-]+/[A-Za-z0-9_.\-]+)(/[A-Za-z0-9_.\-]+)*$]]
        end
    }
}
