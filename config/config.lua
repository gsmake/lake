return {
    SCM = {
        default = "git",
        loaders = {
            git = "gsmake.git"
        }
    },

    sites = {
        ["github.com"] = {
            SCM =    "git",
            URL =    "https://${root}.git",
            Pattern =  [[^(?P<root>github\.com/[A-Za-z0-9_.\-]+/[A-Za-z0-9_.\-]+)(/[A-Za-z0-9_.\-]+)*$]],
        },
        ["gopkg.in"] = {
            SCM =     "git",
            URL =     "https://${root}",
            Pattern = [[^(?P<root>gopkg\.in/[A-Za-z0-9_.\-])$]],
        },

        ["bitbucket.org"] = {
            SCM =     "git",
            URL =     "https://${root}.git",
            Pattern = [[^(?P<root>bitbucket\.org/[A-Za-z0-9_.\-]+/[A-Za-z0-9_.\-]+)(/[A-Za-z0-9_.\-]+)*$]],
        },
    }
}
