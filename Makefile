
#------ Options ------

TRY_MATCH := 1

#------ Aliases ------

EXEPERM := chmod 755
SPLAT := python3 -m splat

#------ Paths ------

ASMDIR := asm
CODEBUILD_DIR := build
CDDIR := cd
ASSETDIR := assets
ASSETSPECDIR := assets/arcspec
TOOLS_DIR := tools
REF_DIR := expected

CDIMG := $(shell find ./ -maxdepth 1 -type f -iregex '.*.iso$' | head -1)
CHECKSUM_FILE := /checksums.csv
CD_BUILD_XML := /isobuild.xml
SPLAT_YAML := /splat.yaml
SPLAT_YAML_BASE := /splatops.yaml
SEC_TABLE := symbols/secs.tsv
VAR_SYMBOL_FILE := symbols/variables.txt
DATA_DUMP_XML := symbols/datadump.xml
EXEBUILD_SPEC_XML := exebuild_spec.xml
MODULE_BUILD_SH := myubuild.sh
MODULE_BUILD_LD := myubuild.ld
KERNEL_SYM_TXT := kernel_syms.txt
EXE_BUILD_ELF := $(CODEBUILD_DIR)/myubuild.bin
LD_MAP_FILE := $(CODEBUILD_DIR)/myubuild.map

ASSET_GEN_DIRS := $(shell find assets/* -type d -not -path "assets/arcspec*")
ASSET_GEN_FILES := $(shell find assets/* -maxdepth 1 -type f -not -path "assets/arcspec*")

MYUPACK_JAR := $(TOOLS_DIR)/myu_packager/myupack.jar
MYUPACK := $(java -Djava.library.path="tools/myu_packager:${env_var:PATH}" -jar $(MYUPACK_JAR))
WIBO := $(TOOLS_DIR)/wibo

LIBLZMU := $(TOOLS_DIR)/myu_packager/liblzmu.so
LIBLZMU_BUILD_SCRIPT := $(TOOLS_DIR)/lz/buildMeGnu.sh

GET_MYUPACK_SCRIPT := $(TOOLS_DIR)/toolfetch/getMyuPack.sh
GET_WIBO_SCRIPT := $(TOOLS_DIR)/toolfetch/getWibo.sh
PYDEP_SCRIPT := $(TOOLS_DIR)/toolfetch/getPyPackages.sh

#------ All ------
#TODO

#------ Cleaning ------

.PHONY: clean cleanasm cleanassets cleanbuild

clean: cleanbuild cleanassets cleanasm
	rm -f splatlink.ld
	rm -f undefined_funcs_auto.txt
	rm -f undefined_syms_auto.txt
	rm -f $(SPLAT_YAML)
	rm -f $(MODULE_BUILD_SH)
	rm -f $(MODULE_BUILD_LD)

cleanbuild:
	rm -rf $(CDDIR)
	rm -rf $(CODEBUILD_DIR)

cleanassets:
	rm -rf $(ASSET_GEN_FILES)
	rm -rf $(ASSET_GEN_DIRS)

cleanasm:
	rm -rf $(ASMDIR)
	
#------ Handle dependencies ------

.PHONY: pydeps

pydeps:
	$(EXEPERM) $(PYDEP_SCRIPT)
	$(PYDEP_SCRIPT)

$(WIBO):
	$(EXEPERM) $(GET_WIBO_SCRIPT)
	$(GET_WIBO_SCRIPT) $(WIBO)

$(LIBLZMU):
	$(EXEPERM) $(LIBLZMU_BUILD_SCRIPT)
	$(LIBLZMU_BUILD_SCRIPT) $(LIBLZMU)

$(MYUPACK_JAR): $(LIBLZMU)
	$(EXEPERM) $(GET_MYUPACK_SCRIPT)
	$(GET_MYUPACK_SCRIPT) $(MYUPACK_JAR)
	$(MYUPACK) testnative

#------ Unpack/Disassemble ------

.PHONY: unpackiso disasm

#Just let the packager break down the archives
unpackiso: $(MYUPACK_JAR)
	$(MYUPACK) isounpack --iso "$(CDIMG)" --cdout "$(CDDIR)" --assetout "$(ASSETDIR)" --arcspec "$(ASSETSPECDIR)" --checksums "$(CHECKSUM_FILE)" --xmlout "$(CD_BUILD_XML)"
	
#Find executable file and break it down
PSX_EXE_FILE := $(shell find ./$(CDDIR) -maxdepth 1 -type f -name '*SLPM_*' | head -1)
PSX_EXE_FILE_BACKUP := $(shell sed 's/$(CDDIR)/$(REF_DIR)/')

$(SPLAT_YAML): $(MYUPACK_JAR) $(SPLAT_YAML_BASE) $(SEC_TABLE)
	$(MYUPACK) splatyaml --exepath "$(PSX_EXE_FILE)" --out "$(SPLAT_YAML)" --sectbl "$(SEC_TABLE)" --ops "$(SPLAT_YAML_BASE)"
	
disasm: $(SPLAT_YAML) pydeps
	$(SPLAT) split "$(SPLAT_YAML)"
	$(MYUPACK) asmsplit --asmdir "$(ASMDIR)" --outdir "$(ASMDIR)/split"
	$(MYUPACK) data2c --xml "$(DATA_DUMP_XML)"
	mkdir $(REF_DIR)
	cp $(PSX_EXE_FILE) $(PSX_EXE_FILE_BACKUP)

#------ Build ------

.PHONY: buildexe packiso
#maybe fetch exe name from SYSTEM.CNF? Since version dependent.

#Arcs need to be built before build script is run so that the res .h and .c files are up to date

$(MODULE_BUILD_SH): $(MYUPACK_JAR) $(EXEBUILD_SPEC_XML)
	$(MYUPACK) genbld --xmlspec "$(EXEBUILD_SPEC_XML)" --builddir "$(CODEBUILD_DIR)" --shout "$(MODULE_BUILD_SH)" --ldout "$(MODULE_BUILD_LD)" --binout "$(EXE_BUILD_ELF)" -gnu
	
$(EXE_BUILD_ELF): $(MODULE_BUILD_SH) $(MODULE_BUILD_LD)
	$(EXEPERM) $(MODULE_BUILD_SH)
	bash $(MODULE_BUILD_SH)
	ifeq ($(TRY_MATCH),1)
		$(MYUPACK) chkobj --sectbl "$(SEC_TABLE)" --builddir "$(CODEBUILD_DIR)/obj" --exefile "$(PSX_EXE_FILE_BACKUP)"
	endif
	mips-linux-gnu-ld -EL -N --no-relax -Map $(LD_MAP_FILE) -T $(MODULE_BUILD_LD) -T $(KERNEL_SYM_TXT) -o $(EXE_BUILD_ELF)


