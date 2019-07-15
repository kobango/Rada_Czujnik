#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Parkingi.mk)" "nbproject/Makefile-local-Parkingi.mk"
include nbproject/Makefile-local-Parkingi.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Parkingi
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SieciowySystemDetekcjiKartaCzujnikowa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SieciowySystemDetekcjiKartaCzujnikowa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=interrupts.c main.c CAN.c EEPROM.c RamkiKarty.c c018i.c RamkiCzujnikow.c WeWy.c transmisja.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/interrupts.o ${OBJECTDIR}/main.o ${OBJECTDIR}/CAN.o ${OBJECTDIR}/EEPROM.o ${OBJECTDIR}/RamkiKarty.o ${OBJECTDIR}/c018i.o ${OBJECTDIR}/RamkiCzujnikow.o ${OBJECTDIR}/WeWy.o ${OBJECTDIR}/transmisja.o
POSSIBLE_DEPFILES=${OBJECTDIR}/interrupts.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/CAN.o.d ${OBJECTDIR}/EEPROM.o.d ${OBJECTDIR}/RamkiKarty.o.d ${OBJECTDIR}/c018i.o.d ${OBJECTDIR}/RamkiCzujnikow.o.d ${OBJECTDIR}/WeWy.o.d ${OBJECTDIR}/transmisja.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/interrupts.o ${OBJECTDIR}/main.o ${OBJECTDIR}/CAN.o ${OBJECTDIR}/EEPROM.o ${OBJECTDIR}/RamkiKarty.o ${OBJECTDIR}/c018i.o ${OBJECTDIR}/RamkiCzujnikow.o ${OBJECTDIR}/WeWy.o ${OBJECTDIR}/transmisja.o

# Source Files
SOURCEFILES=interrupts.c main.c CAN.c EEPROM.c RamkiKarty.c c018i.c RamkiCzujnikow.c WeWy.c transmisja.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-Parkingi.mk dist/${CND_CONF}/${IMAGE_TYPE}/SieciowySystemDetekcjiKartaCzujnikowa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F26K80
MP_PROCESSOR_OPTION_LD=18f26k80
MP_LINKER_DEBUG_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/interrupts.o: interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/interrupts.o.d 
	@${RM} ${OBJECTDIR}/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/interrupts.o   interrupts.c 
	@${DEP_GEN} -d ${OBJECTDIR}/interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/main.o   main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/CAN.o: CAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/CAN.o.d 
	@${RM} ${OBJECTDIR}/CAN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/CAN.o   CAN.c 
	@${DEP_GEN} -d ${OBJECTDIR}/CAN.o 
	@${FIXDEPS} "${OBJECTDIR}/CAN.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/EEPROM.o: EEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/EEPROM.o   EEPROM.c 
	@${DEP_GEN} -d ${OBJECTDIR}/EEPROM.o 
	@${FIXDEPS} "${OBJECTDIR}/EEPROM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/RamkiKarty.o: RamkiKarty.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RamkiKarty.o.d 
	@${RM} ${OBJECTDIR}/RamkiKarty.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/RamkiKarty.o   RamkiKarty.c 
	@${DEP_GEN} -d ${OBJECTDIR}/RamkiKarty.o 
	@${FIXDEPS} "${OBJECTDIR}/RamkiKarty.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/c018i.o: c018i.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/c018i.o.d 
	@${RM} ${OBJECTDIR}/c018i.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/c018i.o   c018i.c 
	@${DEP_GEN} -d ${OBJECTDIR}/c018i.o 
	@${FIXDEPS} "${OBJECTDIR}/c018i.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/RamkiCzujnikow.o: RamkiCzujnikow.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RamkiCzujnikow.o.d 
	@${RM} ${OBJECTDIR}/RamkiCzujnikow.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/RamkiCzujnikow.o   RamkiCzujnikow.c 
	@${DEP_GEN} -d ${OBJECTDIR}/RamkiCzujnikow.o 
	@${FIXDEPS} "${OBJECTDIR}/RamkiCzujnikow.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/WeWy.o: WeWy.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/WeWy.o.d 
	@${RM} ${OBJECTDIR}/WeWy.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/WeWy.o   WeWy.c 
	@${DEP_GEN} -d ${OBJECTDIR}/WeWy.o 
	@${FIXDEPS} "${OBJECTDIR}/WeWy.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/transmisja.o: transmisja.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/transmisja.o.d 
	@${RM} ${OBJECTDIR}/transmisja.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/transmisja.o   transmisja.c 
	@${DEP_GEN} -d ${OBJECTDIR}/transmisja.o 
	@${FIXDEPS} "${OBJECTDIR}/transmisja.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/interrupts.o: interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/interrupts.o.d 
	@${RM} ${OBJECTDIR}/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/interrupts.o   interrupts.c 
	@${DEP_GEN} -d ${OBJECTDIR}/interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/main.o   main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/CAN.o: CAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/CAN.o.d 
	@${RM} ${OBJECTDIR}/CAN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/CAN.o   CAN.c 
	@${DEP_GEN} -d ${OBJECTDIR}/CAN.o 
	@${FIXDEPS} "${OBJECTDIR}/CAN.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/EEPROM.o: EEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/EEPROM.o   EEPROM.c 
	@${DEP_GEN} -d ${OBJECTDIR}/EEPROM.o 
	@${FIXDEPS} "${OBJECTDIR}/EEPROM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/RamkiKarty.o: RamkiKarty.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RamkiKarty.o.d 
	@${RM} ${OBJECTDIR}/RamkiKarty.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/RamkiKarty.o   RamkiKarty.c 
	@${DEP_GEN} -d ${OBJECTDIR}/RamkiKarty.o 
	@${FIXDEPS} "${OBJECTDIR}/RamkiKarty.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/c018i.o: c018i.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/c018i.o.d 
	@${RM} ${OBJECTDIR}/c018i.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/c018i.o   c018i.c 
	@${DEP_GEN} -d ${OBJECTDIR}/c018i.o 
	@${FIXDEPS} "${OBJECTDIR}/c018i.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/RamkiCzujnikow.o: RamkiCzujnikow.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RamkiCzujnikow.o.d 
	@${RM} ${OBJECTDIR}/RamkiCzujnikow.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/RamkiCzujnikow.o   RamkiCzujnikow.c 
	@${DEP_GEN} -d ${OBJECTDIR}/RamkiCzujnikow.o 
	@${FIXDEPS} "${OBJECTDIR}/RamkiCzujnikow.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/WeWy.o: WeWy.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/WeWy.o.d 
	@${RM} ${OBJECTDIR}/WeWy.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/WeWy.o   WeWy.c 
	@${DEP_GEN} -d ${OBJECTDIR}/WeWy.o 
	@${FIXDEPS} "${OBJECTDIR}/WeWy.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/transmisja.o: transmisja.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/transmisja.o.d 
	@${RM} ${OBJECTDIR}/transmisja.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DPARKINGI -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/transmisja.o   transmisja.c 
	@${DEP_GEN} -d ${OBJECTDIR}/transmisja.o 
	@${FIXDEPS} "${OBJECTDIR}/transmisja.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SieciowySystemDetekcjiKartaCzujnikowa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    18f26k80_bootloader.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "18f26k80_bootloader.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/SieciowySystemDetekcjiKartaCzujnikowa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/SieciowySystemDetekcjiKartaCzujnikowa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   18f26k80_bootloader.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "18f26k80_bootloader.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/SieciowySystemDetekcjiKartaCzujnikowa.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Parkingi
	${RM} -r dist/Parkingi

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
