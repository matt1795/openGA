from conans import ConanFile, CMake, tools

class OpenGAConan(ConanFile):
    name = "openga"
    version = "2.1.0"
    description = "Modern C++ library for genetic algorithms"
    url = "https://github.com/matt1795/openga"
    license = "MIT"
    author = "Matthew Knight <mgk1795@gmail.com>"
    generators = "cmake"
    exports_sources = "*"
    settings = "os", "compiler"
    requirements = "asio/1.13.0@bincrafters/stable"

    options = {
        "examples": [True, False]
    }

    default_options = {
        "examples": False
    }

    def build(self):
        stdAllowed = [ '17', 'gnu17', '20', 'gnu20' ]
        if not self.settings.compiler.cppstd in stdAllowed:
            raise Exception("need atleast C++17")

        cmake = CMake(self)
        if self.options.examples:
            cmake.definitions["Examples"] = "ON"
        cmake.verbose = True

        cmake.configure(source_folder=self.source_folder)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        if self.settings.os == "Linux":
            self.cpp_info.libs.append("pthread")
