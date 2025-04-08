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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/MotorPic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/MotorPic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=MotorConfigs.c MotorFunctions.c MotorGlobals.c MotorInterrupts.c MotorPic.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/MotorConfigs.o ${OBJECTDIR}/MotorFunctions.o ${OBJECTDIR}/MotorGlobals.o ${OBJECTDIR}/MotorInterrupts.o ${OBJECTDIR}/MotorPic.o
POSSIBLE_DEPFILES=${OBJECTDIR}/MotorConfigs.o.d ${OBJECTDIR}/MotorFunctions.o.d ${OBJECTDIR}/MotorGlobals.o.d ${OBJECTDIR}/MotorInterrupts.o.d ${OBJECTDIR}/MotorPic.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/MotorConfigs.o ${OBJECTDIR}/MotorFunctions.o ${OBJECTDIR}/MotorGlobals.o ${OBJECTDIR}/MotorInterrupts.o ${OBJECTDIR}/MotorPic.o

# Source Files
SOURCEFILES=MotorConfigs.c MotorFunctions.c MotorGlobals.c MotorInterrupts.c MotorPic.c



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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/MotorPic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24F16KA301
MP_LINKER_FILE_OPTION=,--script=p24F16KA301.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/MotorConfigs.o: MotorConfigs.c  .generated_files/flags/default/51a5157e5973120020e8cabc3580cf9736f40303 .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorConfigs.o.d 
	@${RM} ${OBJECTDIR}/MotorConfigs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorConfigs.c  -o ${OBJECTDIR}/MotorConfigs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorConfigs.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/MotorFunctions.o: MotorFunctions.c  .generated_files/flags/default/24df7084aea0016ea9bbf18dcc608e5d8e4606b3 .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorFunctions.o.d 
	@${RM} ${OBJECTDIR}/MotorFunctions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorFunctions.c  -o ${OBJECTDIR}/MotorFunctions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorFunctions.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/MotorGlobals.o: MotorGlobals.c  .generated_files/flags/default/e20062c717d488c62fa35011ae1d6fe04e17e80e .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorGlobals.o.d 
	@${RM} ${OBJECTDIR}/MotorGlobals.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorGlobals.c  -o ${OBJECTDIR}/MotorGlobals.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorGlobals.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/MotorInterrupts.o: MotorInterrupts.c  .generated_files/flags/default/a0f4cddaa7a91a55c344e1f32a2c4a58f211a3c3 .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorInterrupts.o.d 
	@${RM} ${OBJECTDIR}/MotorInterrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorInterrupts.c  -o ${OBJECTDIR}/MotorInterrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorInterrupts.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/MotorPic.o: MotorPic.c  .generated_files/flags/default/ff200bc0c97a726fbd5bd34e02b85d94b1ca903 .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorPic.o.d 
	@${RM} ${OBJECTDIR}/MotorPic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorPic.c  -o ${OBJECTDIR}/MotorPic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorPic.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/MotorConfigs.o: MotorConfigs.c  .generated_files/flags/default/15ac6ec2b3609768645969f85755fb5277f74e22 .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorConfigs.o.d 
	@${RM} ${OBJECTDIR}/MotorConfigs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorConfigs.c  -o ${OBJECTDIR}/MotorConfigs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorConfigs.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/MotorFunctions.o: MotorFunctions.c  .generated_files/flags/default/903efe4f8883b42d61840773e645cfea065f6337 .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorFunctions.o.d 
	@${RM} ${OBJECTDIR}/MotorFunctions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorFunctions.c  -o ${OBJECTDIR}/MotorFunctions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorFunctions.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/MotorGlobals.o: MotorGlobals.c  .generated_files/flags/default/1aec63301c0f153f21b7d29234817497be3f7f24 .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorGlobals.o.d 
	@${RM} ${OBJECTDIR}/MotorGlobals.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorGlobals.c  -o ${OBJECTDIR}/MotorGlobals.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorGlobals.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/MotorInterrupts.o: MotorInterrupts.c  .generated_files/flags/default/4abf013a2c913b5c6fda13209f7ea6865836adda .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorInterrupts.o.d 
	@${RM} ${OBJECTDIR}/MotorInterrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorInterrupts.c  -o ${OBJECTDIR}/MotorInterrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorInterrupts.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/MotorPic.o: MotorPic.c  .generated_files/flags/default/2f032679b61f39c0666f29e2821f6ea3b16904f2 .generated_files/flags/default/7dab310884e5de58b737751714b62c5515706f0f
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorPic.o.d 
	@${RM} ${OBJECTDIR}/MotorPic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorPic.c  -o ${OBJECTDIR}/MotorPic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorPic.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/MotorPic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/MotorPic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/MotorPic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/MotorPic.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/MotorPic.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
