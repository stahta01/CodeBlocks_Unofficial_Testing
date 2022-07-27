#!/bin/bash

# -----------------------------------------------------------------------------

if [ "$(id -u)" == "0" ]; then
    echo "You are root. Please run again as a normal user!!!"
    exit 1
fi

CurrentDir=${PWD}
# [ -f "Makefile" ] && make clean

if [ ! -f "bootstrap" ]; then
    if [ -f "../bootstrap" ]; then
        cd ..
    else
        if [ -f "../../bootstrap" ]; then
            cd ../..
        else
            echo Could not find bootstrap or ../bootstrap or ../../bootstrap
            cd $CurrentDir
            exit 2
        fi
    fi
fi


if [ -d "${PWD}/src/devel30_64" ]; then
    CB_DEV_DIR=${PWD}/src/devel30_64
fi
if [ -d "${PWD}/src/devel30" ]; then
    CB_DEV_DIR=${PWD}/src/devel30
fi
if [ -d "${PWD}/src/devel31_64" ]; then
    CB_DEV_DIR=${PWD}/src/devel31_64
fi
if [ -d "${PWD}/src/devel31" ]; then
    CB_DEV_DIR=${PWD}/src/devel31
fi
if [ -d "${PWD}/src/devel32_64" ]; then
    CB_DEV_DIR=${PWD}/src/devel32_64
fi
if [ -d "${PWD}/src/devel32" ]; then
    CB_DEV_DIR=${PWD}/src/devel32
fi
if [ ! -d "$CB_DEV_DIR" ]; then
    echo Could not find ${PWD}/src/devel3* directory.
    cd $CurrentDir
    exit 3
fi

if [ -f "${CB_DEV_DIR}/bin/codeblocks" ]; then
    CB_EXE_DIR=${CB_DEV_DIR}/bin
    CB_EXE_NAME=codeblocks
fi    

if [ -f "${CB_DEV_DIR}/codeblocks" ]; then
    CB_EXE_DIR=${CB_DEV_DIR}
    CB_EXE_NAME=codeblocks
fi    

if [ -f "${CB_DEV_DIR}/CodeBlocks" ]; then
    CB_EXE_DIR=${CB_DEV_DIR}
    CB_EXE_NAME=CodeBlocks
fi 

if [ "${CB_EXE_DIR}"  == "" ]; then
    echo Could not find codeblock exe file in the ${CB_DEV_DIR} directory
    cd ${CurrentDir}
    exit 4
fi
echo ${CB_EXE_NAME} exe dir: ${CB_EXE_DIR}

cd ${CB_EXE_DIR}
echo starting: ./${CB_EXE_NAME}  -v --prefix ${PWD}
#export LD_LIBRARY_PATH=${PWD}
#export CODEBLOCKS_DATA_DIR=${CB_DEV_DIR}/share/${CB_EXE_NAME}

## ./${CB_EXE_NAME} -v --prefix=${CB_DEV_DIR}/share/${CB_EXE_NAME}

unset LD_LIBRARY_PATH
./${CB_EXE_NAME} --verbose --debug-log --multiple-instance --personality=debuging

cd ${CurrentDir}