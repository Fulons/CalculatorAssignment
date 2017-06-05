#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Calculation.o \
	${OBJECTDIR}/GeneralHelperFunctions.o \
	${OBJECTDIR}/Operations.o \
	${OBJECTDIR}/StringHelpers.o \
	${OBJECTDIR}/Variable.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/calculator.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/calculator.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/calculator ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Calculation.o: Calculation.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DDEBUG -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Calculation.o Calculation.c

${OBJECTDIR}/GeneralHelperFunctions.o: GeneralHelperFunctions.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DDEBUG -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GeneralHelperFunctions.o GeneralHelperFunctions.c

${OBJECTDIR}/Operations.o: Operations.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DDEBUG -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Operations.o Operations.c

${OBJECTDIR}/StringHelpers.o: StringHelpers.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DDEBUG -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/StringHelpers.o StringHelpers.c

${OBJECTDIR}/Variable.o: Variable.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DDEBUG -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Variable.o Variable.c

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DDEBUG -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
