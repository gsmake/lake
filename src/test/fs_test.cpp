#include <iostream>
#include <lemon/test/test.hpp>
#include <lemon/fs/fs.hpp>

using namespace lemon::fs;

test_(current_directory){

    current_directory();
}


test_(set_current_directory){

    if(exists("./test"))
    {
        remove_file("./test");
    }

    create_directory("./test");

    test_assert(is_directory("./test"));

    set_current_directory("./test");

    set_current_directory("../");

    test_assert(lemon::fs::abs(".") == current_directory());
}


test_(create_symlink_test){

    if(exists("./test"))
    {
        remove_file("./test");
    }

    create_directory("./test");

    if(exists("./test_link"))
    {
        remove_file("./test_link");
    }

    create_symlink("./test","./test_link");

    test_assert(is_directory("./test_link"));
}

test_(filepath) {



    test_assert(lemon::filepath::path("/").string() == "/");

    test_assert(lemon::filepath::path("").string() == "/");

    test_assert(lemon::filepath::path("./").string() == "./");

    test_assert(lemon::filepath::path("./").leaf() == ".");

    test_assert(lemon::filepath::path("./").parent().string() == "./");

    test_assert(lemon::filepath::path("./test/").string() == "./test/");

    test_assert(lemon::filepath::path("c:.").string() == "c:./");

    test_assert(lemon::filepath::path("c:").string() == "c:/");

    test_assert(lemon::filepath::path("c:").leaf() == "c:");

    test_assert(lemon::filepath::path("c:").parent().string() == "c:/");

    test_assert(lemon::filepath::path("c:/test/").string() == "c:/test/");

    test_assert(lemon::filepath::path("c:./2").string() == "c:./2/");

    test_assert(lemon::filepath::path("c:/test/").parent().string() == "c:/");

    test_assert(lemon::filepath::path("c:/test/").leaf() == "test");

    test_assert(lemon::filepath::path("c:/test/..").compress().string() == "c:/");

    test_assert(lemon::filepath::path("..").compress().string() == "/");
}