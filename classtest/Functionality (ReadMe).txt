Usage;
-Command prompt only as of now, that will change later.
-Open "run.txt", edit as if it were a SB program.

Problems;
-"print !!2" will print 2 not 1 (small innaccuracy)
-opengl window sometimes becomes unresponsive
-program uses up too much cpu power (incorrectly throttled)

Compiling;
You'll need glut before you can compile (follow this tutorial, ignore everything he says about visual studio and just put the glut files in the right places)
https://www.youtube.com/watch?v=TH_hA_Sru6Q

I'm also using VS 2013 if that makes any difference.

Todo;
-add ui
-add support for PTC files and not just txt

-DIM
-print string$ value string$
-unclosed quotes

-remove excess parenthesis from math strings on SBarr set as it interferes with isString and isNumeric


Performance related (lower priority);
-improve performance by replacing IF (SBarr[line] == "COMMAND"){}'s with switch statements (doesn't have a huge impact)
-improve performance by using Variable.FindVal() for isFunc() and related commands
-improve performance by replacing all variables with shorter variables on setting SBarr
-improve performance by converting PEMDAS to right to left (I don't know if that's even possible)
-dynamic recompilation

Syntax error checks (lowest priority)
-prevent system variables from being set in equalsSet
-many synatax error checks missing

Changelog;

0.2 (1/30/2016)
-added lowercase support
-PRINT can now print numbers (no variables yet) ("printed" placeholder removed)

0.3 (2/10/2016)
-math string solving added (+ and - only)
-variable support added
-GOTO @LABEL support added

0.4 (2/15/2016)
-added * and / operator support
-INPUT support
-GOSUB RETURN support
-FOR TO STEP NEXT support
-WAIT and VSYNC support
-STRING$ support

0.5 (2/26/2016)
-added parenthesis support
-added % operator support
-added support for all core system variables/variable functions
-added support for all core system strings/string functions
-IF value THEN ... ELSE ...
-IF value THEN @LABEL ELSE @LABEL
-IF value GOTO @LABEL
-ON value GOSUB/GOSUB @LABEL,@SECONDLABEL... added
-fixed multiple bugs
-added additional glitches
-END no longer needed at the end of every program
-can now use spaces and lowercase letters in "  " quote strings
-: colon separator support
-REM
-SWAP
-? being used for PRINT
-added detailed compatibility list to the readme

0.6 (3/5/2016)
-Complete space (and tab) removal implemented (print 1 + 1 instead of print 1+1)
-much much faster (by not using the wrong mode)
-AND OR NOT XOR == != <= >= < > operators added (all operators complete)
-opengl window added
-GCLS
-GLINE

0.7
-GBOX
-GPSET
-GCOLOR
-' instead of REM
-variables strings and labels can now have numbers in their names
-variable arrays and string arrays added
-"Overall improved system stability." (fixed huge issue that randomly broke the program due to an array not being large enough)
-ACLS
-BTRIG() see button mapping in readme
-AND OR XOR NOT now return binary bitwise results and work correctly
-unclosed text strings (PRINT "text) (reverted)
-PRINT "text" and ? "text" now work without spaces (PRINT"text")
-DATA READ RESTORE
-TMREAD and DTREAD
-"Text"=="Text", "Text"!="Text", "Text">"Text", "Text"<"Text", "Text">="Text", "Text"<="Text" logical text operators

Running 3DED;
PNLSTR needs to be put on console [x]
variables with numbers in the name need to work [o]
' instead of REM [o]
arrays [o]
string arrays [o]
GPSET [o]
BUTTON() and BTRIG() [x]
DIM [x]
?"text [o]
?var [o]
?var var string$ var []
LOCATE [x]

Button mapping;
up (1) - up
down (2) - down
left (4) - left
right (8) - right

a (16) - x
b (32) - z
x (64) - s
y (128) - a

l (256) - q
r (512) - e

start (1024) - enter
select (stop program) - escape



Feature support progress;
[] not implemented
[x] implemented but not done
[o] done
[?] i don't know what this is
[0] implemented but irrelevant and inaccurate, sufficient for now

---core--- (do next)----------------
--return values
abs() [o]
asc() [x] not using PTC ascii format
atan() [o]
cancel [o]
chr$() [x] not using PTC ascii format
cos() [o]
date$ [o]
deg() [o]
erl [0]
err [0]
exp() [o] 
false [o]
floor() [o]
freemem [0]
freevar [0]
hex$() [o]
left$() [o]
len() [o]
log [o]
maincntl [o]
maincnth [?]
mid$() [o]
package$ [x] special
pi() [o]
prgname$ [x]
rad() [o]
result [x]
right$() [o]
rnd() [o]
sgn() [o]
sin() [o]
sqr() [o]
str$() [o]
subst$() [o]
sysbeep [x]
tabstep [0]
tan() [o]
time$ [o]
true [o]
val() [o]
version [o]


--operators
and [o] ~A~
not [o] ~N~
or [o] ~O~
xor [o] ~X~
== [x] ~E~
!= [o] ~D~
>= [o] ~S~
<= [o] ~M~
> [o] ~L~
< [o] ~P~
! [0] !!2 equals 2 when it should equal 1
+,-,/,*,% [x] * needs to be implemented for strings
plain numbers [0] not tied to PTC's limitations
variables [o]
variables arrays [o]
strings [o]
string arrays [o]


--core
clear [] needs more effort from me
data [o]
dim []
dtread [o]
else [o]
end [o]
for to step [o]
gosub [o]
goto [o]
if goto [o] 
if goto else [o]
if then @label [o]
if then @label else @label [o]
if then ... else ... [o]
@label [o]
next [o]
on goto [o]
on gosub [o]
read [o]
rem/' [o]
restore [o]
return [o]
rsort [] needs dim
sort [] needs dim
step [o]
stop [0]
swap [o]
then [o]
tmread [o]
to [o]
vsync [x]
wait [o]
var=value [o]
str$="string" [o]


---input--- (do later)-------------------------------------------------------
--return values
btrig() [0]
button() []
inkey$() []
instr() []
keyboard []
tchst []
tchtime []
tchx []
txhy []

--buttons
brepeat []

--input
input [o]
key []
linput []


---graphical--- (do later)----------------------------------------------------
--return values
bgchk() []
chkchr() []
csrx []
csry []
funcno []
gspoit() []
iconchk() []
iconpage []
iconpmax []
iconpuse []
spchk() []
spgetv() []
sphit() []
sphitno []
sphitrc() []
sphitsp() []
sphitt []
sphitx []
sphity []

--graphical
acls [x]

bgclip []
bgcopy []
bgfill []
bgofs []
bgpage []
bgputbgread []

chrinit []
chrread []
chrset []

cls [x]

colinit []
color []
colread []
colset []

gbox [x]
gcircle []
gcls [x]
gcolor [o]
gcopy []
gdrawmd []
gfill []
gline [x]
gpage []
gpaint []
gpset [x]
gprio []
gputchr []

iconset []

locate []
print [x]
pnlstr []
pnltype []

spangle []
spanim []
spchr []
spclr []
spcol []
spcolvec []
sphome []
spofs []
sppage []
spread []
spscale []
spset []
spsetv []

visible []





---file handling--- (do later)-----------------------------------------------
append []
delete []
exec []
load []
new []
rename []
save []


---sound--- (do later)------------------------------------------------------
--return values
bgmchk() []
bgmgetv() []

--sound
beep []
bgmclear []
bgmplay []
bgmprg []
bgmsetv []
bgmstop []
bgmvol []

---editor--- (do likely never)
list []
files []
run []
cont []
reboot []

---wireless--- (do likely never)
recfile []
sendfile []