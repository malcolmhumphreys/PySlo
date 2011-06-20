#!/usr/bin/python

import sys
import PySlo

PySlo.setPath(".:&")
PySlo.setShader(sys.argv[1])

print "\n%s \"%s\"" % (PySlo.typetoStr(PySlo.getType()),
                       PySlo.getName())
for x in xrange(1, PySlo.getNArgs()+1):
    
    param = PySlo.getArgById(x)
    
    s_storage = PySlo.stortoStr(param['storage'])
    s_detail = PySlo.detailtoStr(param['detail'])
    s_type = PySlo.typetoStr(param['type'])
    
    sys.stdout.write("    \"%s\" \"%s %s %s" % (param['name'], s_storage, s_detail, s_type))
    
    if PySlo.isArray(x):
        if param['arraylen'] == 0:
            sys.stdout.write("[]");
        else:
            sys.stdout.write("[%d]" % param['arraylen']);
    
    sys.stdout.write("\"\n")
    sys.stdout.write("\t\tDefault value: ")
    
    if param['type'] == PySlo.TYPE.COLOR:
        sys.stdout.write("\"rgb\" ")
    
    printspace = False
    if param['type'] == PySlo.TYPE.POINT:
        printspace = True
    if param['type'] == PySlo.TYPE.VECTOR:
        printspace = True
    if param['type'] == PySlo.TYPE.NORMAL:
        printspace = True
    if param['type'] == PySlo.TYPE.MATRIX:
        printspace = True
    
    if param['spacename'] != '' and printspace: 
        sys.stdout.write("\"%s\" " % param['spacename'])
    
    if PySlo.isArray(x):
        sys.stdout.write("{")
    
    for i in xrange(0, len(param['default'])):
        
        value = param['default'][i]
        
        if param['type'] == PySlo.TYPE.SCALAR:
            sys.stdout.write("%g" % value)
        
        vectortype = False
        if param['type'] == PySlo.TYPE.POINT:
            vectortype = True
        if param['type'] == PySlo.TYPE.VECTOR:
            vectortype = True
        if param['type'] == PySlo.TYPE.NORMAL:
            vectortype   = True
        if param['type'] == PySlo.TYPE.COLOR:
            vectortype   = True
        if vectortype:
            sys.stdout.write("[%g %g %g]" % (value[0], value[1], value[2]))
        
        if param['type'] == PySlo.TYPE.MATRIX:
            sys.stdout.write("[%g " % value[0])
            for ii in xrange(1, 15):
                sys.stdout.write("%g " % value[ii])
            sys.stdout.write("%g]" % value[15])
        
        if param['type'] == PySlo.TYPE.STRING:
            sys.stdout.write("\"%s\"" % value)
        
        if param['type'] == PySlo.TYPE.SHADER:
            sys.stdout.write("\"(nil)\"")
        
        if i != len(param['default']) - 1:
            sys.stdout.write(", ")
    
    if PySlo.isArray(x):
        sys.stdout.write("}")
    
    sys.stdout.write("\n")
    
sys.stdout.write("\n")

PySlo.endShader()