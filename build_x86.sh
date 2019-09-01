#!/usr/bin/env bash
_pwd="$(pwd)"

MY_MAKE="make"
MY_BUILD_PATH="./build"
MY_INSTALL_PATH="./install"

HAVING_FORMAT=false
HAVING_ECLIPSE=false
HAVING_DEBUG=false
HAVING_SANITIZER=false
HAVING_VERBOSE=false

function usage()
{
    echo "Library Building Script"
    echo ""
    echo "${0} [options] where [options] are:"
    echo "     -h --help"
    echo "     -verbose --verbose"
    echo "     -install --install"
    echo "     -debug --debug"
    echo "     -sanitize --sanitize"
    echo ""
}

case $OSTYPE in
   'msys')
        MY_BUILD_PATH="build-mingw"
        MY_BUILDER="-G \"MinGW Makefiles\" ${MY_CMAKE_OPTS}"
        # Avoid "sh in Path" error from CMake
        #  https://stackoverflow.com/questions/41333215/mingw-sh-exe-must-not-be-in-your-path
        MY_CMAKE_OPTS+=" -DCMAKE_SH=\"CMAKE_SH-NOTFOUND\""
        MY_MAKE="mingw32-make"
        ;;
    *) ;;
esac


while [ "$1" != "" ]; do
    param=`echo $1 | awk -F= '{print $1}'`
    value=`echo $1 | awk -F= '{print $2}'`

    case $param in
        -h | --help)
            usage
            exit
            ;;
        -verbose | --verbose)
            HAVING_VERBOSE=true
            ;;
        -debug | --debug)
            HAVING_DEBUG=${value}
            ;;
        -sanitize | --sanitize)
            HAVING_SANITIZER=true
            ;;
        -install | --install)
            MY_INSTALL_PATH="$(pwd)/$value"
            ;;
        -ninja | --ninja)
            MY_BUILDER+="-GNinja"
            MY_MAKE="ninja"
            MY_BUILD_PATH="build-ninja"
            ;;
        -codeblocks | --codeblocks)
            MY_BUILDER="-G \"CodeBlocks - Unix Makefiles\" "
            if [ $OSTYPE == "msys" ] ; then
                MY_BUILDER="-G \"CodeBlocks - MinGW Makefiles\" "
            fi
            MY_BUILD_PATH="build-cb"
            ;;
        -clang | --clang)
            export CC=clang
            export CXX=clang++

            MY_BUILD_PATH="build-clang"
            ;;
        *)
            ;;
    esac

    shift
done

MY_CMAKE_OPTS+=" -DCMAKE_INSTALL_PREFIX=\"${MY_INSTALL_PATH}\""

if ${HAVING_SANITIZER}; then
    MY_CMAKE_OPTS+=" -DWITH_SANITIZER=ON"
else
    MY_CMAKE_OPTS+=" -DWITH_SANITIZER=OFF"
fi

if ${HAVING_VERBOSE}; then
    MY_CMAKE_OPTS+=" -DCMAKE_VERBOSE_MAKEFILE=ON"
fi

if ${HAVING_DEBUG}; then
    MY_CMAKE_OPTS+=" -DCMAKE_BUILD_TYPE=Debug"
else
    MY_CMAKE_OPTS+=" -DCMAKE_BUILD_TYPE=Release"
fi

MY_CMAKE_OPTS="${MY_BUILDER} + ${MY_CMAKE_OPTS}"

clear
echo "MY_BUILD_PATH : ${MY_BUILD_PATH}"
echo "MY_CMAKE_OPTS : ${MY_CMAKE_OPTS}"

mkdir -p "${MY_BUILD_PATH}" && cd "${MY_BUILD_PATH}"
eval cmake ${MY_CMAKE_OPTS} ..

if ${HAVING_FORMAT}; then
    ${MY_MAKE} clangformat    # autoformatting code style
    #${MY_MAKE} clangtidy      # C++ linter
    #${MY_MAKE} cppcheck       # C / C++ static analysis
else
    ${MY_MAKE} install
fi

cd "${_pwd}"
