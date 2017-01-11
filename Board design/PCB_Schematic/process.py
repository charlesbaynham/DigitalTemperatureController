#
#
# This file will replace all instances of various strings with their counterparts, listed below in "mapping"
#
#

filename = "TempCtrl.kicad_PCB"

with open (filename, "r") as myfile:
    data=myfile.read()
    myfile.close



mapping = {
		'R7' : 'R33',
		'C5' : 'C46',
		'R4' : 'R30',
		'C7' : 'C47',
		'R6' : 'R32',
		'C4' : 'C45',
		'R5' : 'R31',
		'U3' : 'U17'
	}


for k, v in mapping.iteritems():
    data = data.replace('reference ' + k, 'reference ' + v)

# print data

with open (filename, "w") as myfile:
	myfile.write(data)