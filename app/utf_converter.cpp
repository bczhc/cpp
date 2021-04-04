//
// Created by bczhc on 4/4/21.
//

#include "../utf8.hpp"
#include "../string.hpp"
#include "../file.hpp"
#include <cstdio>
#include <cstdint>
#include "../utils.hpp"
#include <cassert>
#include <csignal>

using namespace bczhc;

using UnicodeConverter = void (*)(uint32_t codepoint, Endianness endianness);

static Endianness selfEndianness = getEndianness();

void handleUtf16Input(UnicodeConverter converter, Endianness fromEndianness, Endianness toEndianness);

void handleUtf32Input(UnicodeConverter converter, Endianness fromEndianness, Endianness toEndianness);

void putUnicode2utf8(uint32_t codepoint, Endianness);

void putUnicode2utf16(uint32_t codepoint, Endianness endianness);

void putUnicode2utf32(uint32_t codepoint, Endianness endianness);

void handleUtf8Input(UnicodeConverter converter, Endianness toEndianness);

void signalHandler(int signal) {
    if (signal == SIGINT) {
        fflush(stdout);
        fclose(stdout);
    }
}

int main(int argc, char **argv) {
    const String selfFilename = File::getFileName(argv[0]);
    String formatMsg = "A from-stdin-to-stdout UTF converter.\nUsage: %s [option] <from> <to>\n       %s (--help | -h)\n       %s --info\n\nOptions:\n  -b <size>, --buffer-size <size>  IO buffer size, in bytes.\n  -i <path>, --input <path>  Input file instead of stdin.\n  -o <path>, --output <path>  Output file instead of stdout.\n\nPositional arguments:\nfrom, to: [utf[-]](8|16be|16le|32be|32le)\n";
    size_t newHelpMsgSize = formatMsg.length() - 2 * 3 + selfFilename.length() * 3 + 1;
    char helpMsg[newHelpMsgSize];
    const char *fn = selfFilename.getCString();
    sprintf(helpMsg, formatMsg.getCString(), fn, fn, fn);
    helpMsg[newHelpMsgSize - 1] = '\0';

    if (argc == 1) {
        puts(helpMsg);
        return 0;
    }

    if (argc == 2) {
        String argv1 = argv[1];
        if (argv1.equals("-h") || argv1.equals("--help")) {
            puts(helpMsg);
            return 0;
        } else if (argv1.equals("--info")) {
            puts("Written by bczhc (https://github.com/bczhc).\n...\n");
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[1]);
            return 1;
        }
    }

    int64_t bufferSize = 8192;
    // null means stdin
    String inputFilePath = nullptr;
    // null means stdout
    String outputFilePath = nullptr;

    if (argc > 3) {
        for (int i = 1; i < argc - 2; i += 2) {
            String option = argv[i];
            if (option.charAt(0) != '-' && !option.substr(0, 2).equals("--")) {
                fprintf(stderr, "Unknown option: %s\n", option.getCString());
                return 1;
            }
            if (i + 1 >= argc) {
                fprintf(stderr, "Invalid arguments count.\n");
                return 1;
            }
            String arg = argv[i + 1];

            if (option.equals("-b") || option.equals("--buffer-size")) {
                try {
                    bufferSize = Long::parseLong(arg);
                } catch (const NumberFormatException &e) {
                    fprintf(stderr, "%s\n", e.what());
                    return 1;
                }
            } else if (option.equals("-i") || option.equals("--input")) {
                inputFilePath = arg;
            } else if (option.equals("-o") || option.equals("--output")) {
                outputFilePath = arg;
            } else {
                fprintf(stderr, "Unknown option: %s\n", option.getCString());
                return 1;
            }
        }
    }

    signal(SIGINT, signalHandler);

    if (!inputFilePath.isNull()) {
        freopen64(inputFilePath.getCString(), "rb", stdin);
    }
    if (!outputFilePath.isNull()) {
        freopen64(outputFilePath.getCString(), "wb", stdout);
    }

    String from = String::toLowerCase(argv[argc - 2]), to = String::toLowerCase(argv[argc - 1]);
    UnicodeConverter converter;
    Endianness toEndianness{};

    if (to.equals("utf8") || to.equals("utf-8")) {
        converter = putUnicode2utf8;
    } else if (to.equals("utf16be") || to.equals("utf-16be")) {
        converter = putUnicode2utf16;
        toEndianness = Endianness::BIG;
    } else if (to.equals("utf16le") || to.equals("utf-16le")) {
        converter = putUnicode2utf16;
        toEndianness = Endianness::LITTLE;
    } else if (to.equals("utf32be") || to.equals("utf-32be")) {
        converter = putUnicode2utf32;
        toEndianness = Endianness::BIG;
    } else if (to.equals("utf32le") || to.equals("utf-32le")) {
        converter = putUnicode2utf32;
        toEndianness = Endianness::LITTLE;
    } else {
        fprintf(stderr, "Unknown <to> encode: %s\n", argv[2]);
        return 1;
    }

    if (from.equals("utf8") || from.equals("utf-8")) {
        handleUtf8Input(converter, toEndianness);
    } else {
        const char *unknownFromOptionFormat = "Unknown <from> option: %s\n";
        if (from.length() != 7 && from.length() != 8) {
            fprintf(stderr, unknownFromOptionFormat, argv[1]);
            return 1;
        }
        if (!from.substr(0, 3).equals("utf")) {
            fprintf(stderr, unknownFromOptionFormat, argv[1]);
            return 1;
        }
        char c = from.charAt(3);
        if (c != '-' && c != '1' && c != '3') {
            fprintf(stderr, unknownFromOptionFormat, argv[1]);
            return 1;
        }

        String mid2, end2;
        if (c == '-') {
            mid2 = from.substr(4, 2);
            end2 = from.substr(6, 2);
        } else {
            mid2 = from.substr(3, 2);
            end2 = from.substr(5, 2);
        }

        if (mid2.equals("16")) {
            if (end2.equals("be")) {
                // utf-16be
                handleUtf16Input(converter, Endianness::BIG, toEndianness);
            } else if (end2.equals("le")) {
                handleUtf16Input(converter, Endianness::LITTLE, toEndianness);
            } else {
                fprintf(stderr, unknownFromOptionFormat, argv[1]);
                return 1;
            }
        } else if (mid2.equals("32")) {
            if (end2.equals("be")) {
                handleUtf32Input(converter, Endianness::BIG, toEndianness);
            } else if (end2.equals("le")) {
                handleUtf32Input(converter, Endianness::LITTLE, toEndianness);
            } else {
                fprintf(stderr, unknownFromOptionFormat, argv[1]);
                return 1;
            }
        } else {
            fprintf(stderr, unknownFromOptionFormat, argv[1]);
            return 1;
        }
    }
    return 0;
}

void handleUtf8Input(UnicodeConverter converter, Endianness toEndianness) {
    SolvedUTF8Properties solvedProperties;
    uchar read[4];

    while (true) {
        if (fread(read, 1, 1, stdin) <= 0) break;
        int bytesLength = getUTF8BytesLength(read[0]);
        if (bytesLength != 1) {
            size_t readLen = fread(read + 1, 1, bytesLength - 1, stdin);
            assert(readLen == bytesLength - 1);
        }
        solveUTF8Bytes(solvedProperties, read);

        converter(solvedProperties.codepoint, toEndianness);
    }
}

void putUnicode2utf8(uint32_t codepoint, Endianness) {
    char utfBytes[4];
    int utf8Size = unicode2UTF8(utfBytes, codepoint);
    for (int i = 0; i < utf8Size; ++i) {
        putchar(utfBytes[i]);
    }
}

void putUnicode2utf16(uint32_t codepoint, Endianness endianness) {
    if (codepoint < 0xffffu) {
        codepoint &= 0xffffu;
        auto b = ((char *) &codepoint);
        if (selfEndianness == endianness) {
            putchar(b[0]), putchar(b[1]);
        } else {
            putchar(b[1]), putchar(b[0]);
        }
    } else {
        codepoint -= 0x10000u;
        uint16_t lowSurrogate = (codepoint & 0b1111111111u) + 0xdc00u;
        uint16_t highSurrogate = (codepoint >> 10u) + 0xd800u;
        char *b;
        b = (char *) &highSurrogate;
        if (selfEndianness == endianness) {
            putchar(b[0]), putchar(b[1]);
            b = (char *) &lowSurrogate;
            putchar(b[0]), putchar(b[1]);
        } else {
            putchar(b[1]), putchar(b[0]);
            b = (char *) &lowSurrogate;
            putchar(b[1]), putchar(b[0]);
        }
    }
}

void putUnicode2utf32(uint32_t codepoint, Endianness endianness) {
    auto b = (char *) &codepoint;
    if (selfEndianness == endianness) {
        putchar(b[0]);
        putchar(b[1]);
        putchar(b[2]);
        putchar(b[3]);
    } else {
        putchar(b[3]);
        putchar(b[2]);
        putchar(b[1]);
        putchar(b[0]);
    }
}

void handleUtf16Input(UnicodeConverter converter, Endianness fromEndianness, Endianness toEndianness) {
    uint16_t read, t, highSurrogate;

    while (true) {
        if (fread(&read, 1, 2, stdin) <= 0) break;

        if (fromEndianness != selfEndianness) {
            t = read;
            ((char *) &read)[0] = ((char *) &t)[1];
            ((char *) &read)[1] = ((char *) &t)[0];
        }

        if (read >= 0xd800u && read <= 0xdb7fu) {
            // surrogate
            highSurrogate = read - 0xd800u;
            size_t readLen = fread(&read, 1, 2, stdin);
            assert(readLen == 2);
            if (fromEndianness != selfEndianness) {
                t = read;
                ((char *) &read)[0] = ((char *) &t)[1];
                ((char *) &read)[1] = ((char *) &t)[0];
            }
            converter((((uint32_t) highSurrogate) << 10u) | read, toEndianness);
        } else {
            auto codepoint = (uint32_t) read;
            converter(codepoint, toEndianness);
        }
    }
}

void handleUtf32Input(UnicodeConverter converter, Endianness fromEndianness, Endianness toEndianness) {
    uint32_t read, t;
    while (fread(&read, 1, 4, stdin) > 0) {
        if (fromEndianness != selfEndianness) {
            t = read;
            ((char *) &read)[0] = ((char *) &t)[3];
            ((char *) &read)[1] = ((char *) &t)[2];
            ((char *) &read)[2] = ((char *) &t)[1];
            ((char *) &read)[3] = ((char *) &t)[0];
        }
        converter(read, toEndianness);
    }
}
