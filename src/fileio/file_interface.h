#pragma once
#include <fstream>

class FileHandlerInterface {
    public:
        virtual void open( std::string filename) { };
        virtual std::istream& read (char* s, std::streamsize n) {};
        virtual bool is_open() {};
        virtual std::streamsize gcount() {};
        FileHandlerInterface() {};
        virtual ~FileHandlerInterface() = default;
};

class FileHandler : public FileHandlerInterface {
    std::ifstream f;
    public:
        FileHandler() {
        }

        void open(std::string filename) override {
            f.open(filename);
        }

        std::istream& read(char* s, std::streamsize n) override {
            return f.read(s, n);
        }
        bool is_open() override {
            return f.is_open();
        }
        std::streamsize gcount() override {
            return f.gcount();
        }
};

class TestFileHandler : public FileHandlerInterface {
    public:
        std::string source;
        TestFileHandler(std::string source) {
            this->source = source;
        }

        void open(std::string filename) override {
            // no-op
        }

        std::istream& read(char* s, std::streamsize n) override {
            source.copy(s, 4096);
        }

        bool is_open() override {
            return true;
        }

        std::streamsize gcount() override {
            return source.size();
        }
};