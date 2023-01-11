CSRC_HOME := $(shell if [ "$$CSRC_HOME" != "" ]; then echo $$CSRC_HOME; else echo "$$PWD/.."; fi)
include $(CSRC_HOME)/config.mk

LIBS	=	
PCPGM	=	PushProcessV2
CPGM	=	ReceiveProcess PushProcessV1 MQTTPusherCliV1 MQTTPusherCliV2

LINK    =   -L$(ORACLE_HOME)/lib/ -lm -lz -lclntsh -lpthread -L$(LIB_PATH) -lMQTTPusher -L$(SCLIB_PATH)/lib -lSC -ljson-c

all	:	$(PCPGM) \
		$(CPGM) \
		clean

test:	$(TSETPGM)

$(LIBS) :	$@
		$(CC) $(COMP) $@.c

$(PCPGM) :	$@
		$(PROC) $(PFLAGS) iname=$@
		$(CC) $(COMP) $(ORA_COMP) $@.c
		$(CC) -o $(INSTALL_PATH)/$@ $@.o $(LIB) $(LINK)
		rm -f $@.c

$(CPGM) :	$@
		$(CC) $(COMP) $@.c
		$(CC) -o $(INSTALL_PATH)/$@ $@.o $(LIB) $(LINK)

clean   :
		rm -f *.o *.lis
