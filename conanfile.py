from conans import ConanFile, CMake, tools

class OpenGAConan(ConanFile):
    name = "openGA"
    version = "2.0"
    license = "MIT"
    author = "Matthew Knight <mgk1795@gmail.com>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Openga here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "*"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="hello")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
