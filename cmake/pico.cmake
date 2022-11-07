message("found pico.cmake")

add_executable(TestInstantiator TestInstantiator.cxx)
add_test(NAME TestInstantiator
         COMMAND TestInstantiator)
