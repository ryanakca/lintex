$! Last Rev.: MLO 1996-03-18
$! File: LINTEX.COM --- To delete TeX-related garbage (files with extensions
$!			.DVI, .AUX, .TOC, ...)
$!	@LINTEX 	deletes TeX-related files only if a .TEX exists
$!	@LINTEX F	('force') deletes TeX-related files anyway
$!	@LINTEX C	('confirm') asks user if a .TEX does not exists
$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$!
$	ver = f$verify(0)
$	say := write sys$output
$!
$	if p1 .eqs. ""
$	    then
$		opt = ""
$	    else
$		opt = f$extract(0, 1, f$edit(p1, "upcase"))
$		if opt.nes."F" .and. opt.nes."C"
$		    then
$			say "''opt': invalid option"
$			exit
$		    endif
$	    endif
$!
$	filext = "DVI/AUX/TOC/LIS/LOF/LOT/PS"
$	numext = 0
$!
$	purge /log
$!
$ eloop: ext = f$element(numext, "/", filext)
$	if ext .nes. "/"
$	    then
$		filnam = "*." + ext
$ floop:	file = f$search(filnam, 1)
$		if file .nes. ""
$		    then
$			first = f$locate("]", file)
$			len = f$length(file)
$			if first .eq. len
$			    then
$				short = file
$			    else
$				short = f$extract(first+1, len, file)
$			    endif
$			filedate = f$cvtime(f$file(file, "cdt"))
$			gosub delchk
$			goto floop
$		    endif
$		numext = numext + 1
$		goto eloop
$	    endif
$!
$	if ver then set verify
$	exit
$!
$! SUBROUTINE DELCHK
$!
$ delchk:
$	texfile = f$parse(".tex;", file)
$	tex = f$search(texfile, 2)
$	if tex .eqs. ""
$	    then
$		errmes = "a .TEX file does not exist."
$		gosub chkopt
$	    else
$		texdate = f$cvtime(f$file(tex, "cdt"))
$		if texdate .les. filedate
$		    then
$			delete /log 'file'
$		    else
$			errmes = "the .TEX file is newer."
$			gosub chkopt
$		    endif
$	    endif
$	return
$!
$! SUBROUTINE CHKOPT
$!
$ chkopt:
$	if opt.eqs."F"
$	    then
$		delete /log 'file'
$	    else
$		if opt.eqs."C"
$		    then
$			delete /log /confirm 'file'
$		    else
$			say "--> ''short' not deleted; ''errmes'"
$		    endif
$	    endif
$	return
