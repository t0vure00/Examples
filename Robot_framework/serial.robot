*** Settings ***
Library  SerialLibrary
Library  OperatingSystem
Library  String

*** Variables ***
${com}   COM4
${board}   FRDM

*** Test Cases ***
	
Connect Serial
	Log To Console  Connecting to ${board}
	Add Port  ${com}  baudrate=9600   encoding=ascii
	Port Should Be Open  ${com}
	Reset Input Buffer
	Reset Output Buffer

Test Message Identifier
	Write to Serial Zero
	${read} =   Read Until  terminator=0  encoding=ascii
	Should Be True  ${read} == 1
	Write to Serial One
	${read} =   Read Until  terminator=0  encoding=ascii
	Should Be True  ${read} == 2
	Write to Serial Two
	${read} =   Read Until  terminator=0  encoding=ascii
	Should Be True  ${read} == 3
	Write to Serial Three
	${read} =   Read Until  terminator=0  encoding=ascii
	Should Be True  ${read} == 4
	Write to Serial Four
	${read} =   Read Until  terminator=0  encoding=ascii
	Should Be True  ${read} == 5
	Write to Serial Five
	${read} =   Read Until  terminator=0  encoding=ascii
	Should Be True  ${read} == 6
	Write to Serial Six
	${read} =   Read Until  terminator=0  encoding=ascii
	Should Be True  ${read} == 0

Disconnect Serial
	Log To Console  Disconnecting ${board}
	[TearDown]  Delete Port  ${com}

*** Keywords ***
Write to Serial Zero
	Log To Console  zero field wrong
    Write Data  $GPGA,134732.000,5540.3244,N,01231.2941,E${\n}  encoding=ascii

Write to Serial One
	Log To Console  first field wrong
    Write Data  $GPGGA,0.000,5540.3244,N,01231.2941,E${\n}  encoding=ascii

Write to Serial Two
	Log To Console  second field wrong
    Write Data  $GPGGA,134732.000,5556740.3244,N,01231.2941,E${\n}  encoding=ascii

Write to Serial Three
	Log To Console  third field wrong
    Write Data  $GPGGA,134732.000,5540.3244,P,01231.2941,E${\n}  encoding=ascii

Write to Serial Four
	Log To Console  fourth field wrong
    Write Data  $GPGGA,134732.000,5540.3244,N,233491231.2941,E${\n}  encoding=ascii

Write to Serial Five
	Log To Console  fifth field wrong
    Write Data  $GPGGA,134732.000,5540.3244,N,01231.2941,W${\n}  encoding=ascii

Write to Serial Six
	Log To Console  no fields wrong
    Write Data  $GPGGA,134732.000,5540.3244,N,01231.2941,E${\n}  encoding=ascii