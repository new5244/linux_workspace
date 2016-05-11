How to compile CDR Mate 4 linux console 

1. build minIni library (opensource ini file parser)
	cd minini
	make clean
	make
	cp minIni.h minGlue.h ../src


2. build console 
	cd src
	make clean
	make dep
	make


* Source code version control with Mercurial (Tortoise HG is Good)
