# MiniLibrary
  4 
Topic	4.			15	points	(Group,	Implemented	in	C)	
Mini	Library		
Create	a	system	managing	a	mini	library	system.	The	users	include	borrower	and	librarian.	
Each	user	corresponds	to	a	record	(line)	in	a	text	file	named	“UserInfo.txt”.	Each	record	
consists	of	(User	ID,	First	Name,	Last	Name,	Account	Name,	Password,	Role).	The	role	‘B’	
represents	borrower	while	‘L’	represents	librarian.	Assume	users’	information	have	been	
added	to	the	file	“UserInfo.txt”	before	the	execution	of	the	system.		
	
	  	UserInfo.txt	
1,First1	,Last1,	Acct1,1234,B	 		
2,First2	,Last2,	Acct2,	abcd,B	
3,First3	,Last3,	Acct3,	fffff,L	 	
	
	
Every	book	corresponds	to	a	record	(line)	in	a	text	file	named	“mylibrary.txt”.	Each	record	
consists	of	6	fields	(Book	ID,	Title,	Author,	Possession,	Check	out	Date,	Due	Date)	separated	
by	comma:	
Mylibrary.txt	
1,Book1,Author1,Library,null,null	 		 	
2,Book1,Author1,User1,2014-10-22,2014-11-21	
3,Book2,Author2,User1,2014-10-23,2014-11-22	
4,Book3,Author2,User2,2014-10-23,2014-11-22	
5,Book4,Atuhor3,User2,2014-09-22,2014-10-22	
	
l  No	comma	“,”in	title	or	author	name.		
l  This	 mini	 library	 keeps	 the	 record	 for	 each	 book	 in	 the	 library.	 Different	 books	 can	
share	the	book	“Title”.	But	the	“Book	ID”	for	each	book	is	unique.		
l  One	 author	 may	 have	 multiple	 books.	 E.g.	 Both	 book2	 and	 book3	 are	 written	 by	
author1		
l  Possession	 field	 is	 “Library”	 means	 this	 book	 is	 kept	 in	 library,	 such	 as	 book	 #001.	
 Otherwise,	 this	 book	 is	 checked	 out	 by	 the	 corresponding	 user.	 E.g.	 book	 #3	 is	
checked	out	by	User1.		
l  A	user	is	allowed	to	check	out	up	to	3	books.		
l  The	“Checked	out	Date”	field	specifies	the	time	of	the	latest	checked	out	date.	The	loan	
period	for	each	book	is	30	days.	“The	Due	Data”	filed	specifies	the	time	the	book	should	
be	returned.		
l  This	system	asks	the	user	to	login	by	entering	the	correct	account	name	and	password.		
	
If	 the	 user	 is	 a	 borrower,	 it	 provides	 a	 menu	 with	 following	 functionalities	 that	 the	
borrower	can	choose	from	at	the	top-level	menu:		
	
1.  Enter	“q”	for	book	query	by	author	sorted	by	book	title.		
•  1)	Ask	for	author		
•  2)	List	records	of	all	books	written	by	this	author	sorted	by	the	book	title.		  5 
•  3)	Provide	options	“t”	for	“Try	again”	or	“b”	for	“Back	to	main	menu”.	(If	“Try	
again”,	go	to	step	1))	
	
2.  Enter	“s”	for	book	status	query	by	name		
•  1)	Ask	for	book	title.		
•  2)	Examine	if	this	book	exists.	If	no,	print	a	message	“No	such	book!”	and	then	go	
to	4).	 Otherwise,	3)		
•  3)	List	the	possession	status	of	these	books.	If	the	book	is	available,	report	it	is	in	
library;	otherwise,	report	the	available	date	to	borrow	it	(the	due	date	for	this	
book).	
e.g.	
1	Book1	In	library	
2	Book1	Available	after	2014-11-21	
•  4)	Provide	options	“t”	for	“Try	again”	or	“b”	for	“Back	to	main	menu”.	(If	“Try	
again”,	go	to	step	1))	
	
3.  Enter	“u”	to	list	books	checked	out	by	a	given	user		
•  1)	 Ask	 for	 current	 date	 and	 check	 if	 it	 is	 a	 legal	 date.	 If	 illegal,	 output	 “illegal	
input	date”	and	go	to	step	4);	otherwise,	2).	
•  2)	List	all	books	currently	checked	out	by	the	given	user.	If	the	due	date	is	passed,	
marked	the	record	as	“Over	Due”.		
e.g.	Assuming	current	date	is	2014-12-01,for	“User1”		
2,Book1,Author1,User1,2014-10-22,2014-11-21		Over	Due	
3,Book2,Author2,User1,2014-10-23,2014-11-22		Over	Due	
•  3)	Provide	options	“t”	for	“Try	again”	or	“b”	for	“Back	to	main	menu”.	(If	“Try	
again”,	go	to	step	(1))	
	
If	 the	 user	 is	 a	 librarian,	 it	 provides	 a	 menu	 with	 following	 functionalities	 that	 the	
librarian	can	choose	from	at	the	top-level	menu:		
	
1.  Enter	“a”	to	add	a	book	into	library		
•  1)	Ask	for	title	(read).		
•  2)	Ask	for	author.	
•  3)	Generate	a	book	ID	by	adding	one	to	the	current	largest	book	ID.	
•  4)	 Add	 the	 record	 of	 the	 new	 book	 into	 library	 with	 generated	 book	 ID,	 title,	
author,	possession=”library”,	checked	out	date=”null”	and	due	date=”null”.	After	
that,	print	message	“book***	added	successfully!”	and	go	to	step	5).		
•  5)	Provide	options	“t”	for	“Try	again”	and	“b”	for	“Back	to	main	menu”.	(If	“Try	
again”,	go	to	step	1))	
	
	
2.  Enter	“d”	to	delete	a	book	from	library		
•  1)	Ask	for	book	ID.		
•  2)	Examine	if	this	book	exists.	If	no,	print	a	message	“No	such	book!”	and	then	go	
to	4).	Otherwise,	3)		
•  3)	Remove	the	book	with	the	given	book	ID	from	the	library	and	then	go	to	4).		•  4)	Provide	options	“t”	for	“Try	again”	or	“b”	for	“Back	to	main	menu”.	(If	“Try	
again”,	go	to	step	1))	
	
3.  Enter	“o”	to	check	out	a	book		
•  1)	Ask	for	user	name.		
•  2)	Count	how	many	books	this	user	has	already	checked	out,	only	proceed	the	
current	checkout	if	less	than	3	(go	to	3)).	Otherwise,	go	to	5)		
•  3)	Ask	for	book	Id,	examine	if	this	book	exists.	If	no,	print	a	message	“No	such	
book!”	and	then	go	to	5).	Otherwise,	4)		
•  4)	Update	 the	 possession,	 “check	 out	 date”	 and	 “due	 date”	 field	 of	 the	
corresponding	 book	 record.	 After	 that,	 print	 message	 “book***	 checked	 out	
successfully!”	and	then	go	to	5).		
•  5)	Provide	options	“t”	for	“Try	again”	or	“b”	for	“Back	to	main	menu”.	(If	“Try	
again”,	go	to	step	1))	
	
4.  Enter	“r”	to	return	a	book		
•  1)	Ask	for	book	ID.		
•  2)	Examine	if	this	book	exists.	If	no,	print	a	message	“No	such	book!”	and	then	go	
to	4).	Otherwise,	3)		
•  3)	 Ask	 for	 current	 date	 and	 check	 if	 it	 is	 a	 legal	 date.	 If	 illegal,	 output	 “illegal	
input	date”	and	go	to	step	7);	otherwise,	4).	
•  4)	Check	if	current	date	has	passed	“Due	Date”.	If	no,	go	to	step	6).	Otherwise,	5)	
•  5)	Output	the	current	fine	to	this	book.	
•  6)	Assume	user	has	already	paid	for	fine	if	it	exists.	Update	the	possession	and	
date	 fields	 of	 the	 corresponding	 record.	 After	 that,	 print	 message	 “book***	
return	successfully!”		and	then	go	to	7).		
•  7)	Provide	options	“t”	for	“Try	again”	or	“b”	for	“Back	to	main	menu”.	(If	“Try	
again”,	go	to	step	1))	
	
	
5.  Enter	“x”	to	quit.		
