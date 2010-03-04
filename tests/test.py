#!/usr/bin/python

import sys
import PySlo

PySlo.setPath(".:&")
PySlo.setShader(sys.argv[1])

print "\n%s \"%s\"" % (PySlo.getType(), PySlo.getName())
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
    
    if param['type'] == PySlo.type.color:
        sys.stdout.write("\"rgb\" ")
    
    printspace = False
    if param['type'] == PySlo.type.point:
        printspace = True
    if param['type'] == PySlo.type.vector:
        printspace = True
    if param['type'] == PySlo.type.normal:
        printspace = True
    if param['type'] == PySlo.type.matrix:
        printspace = True
    
    if param['spacename'] != '' and printspace: 
        sys.stdout.write("\"%s\" " % param['spacename'])
    
    if PySlo.isArray(x):
        sys.stdout.write("{")
    
    for i in xrange(0, len(param['default'])):
        
        value = param['default'][i]
        
        if param['type'] == PySlo.type.scalar:
            sys.stdout.write("%g" % value)
        
        vectortype = False
        if param['type'] == PySlo.type.point:
            vectortype = True
        if param['type'] == PySlo.type.vector:
            vectortype = True
        if param['type'] == PySlo.type.normal:
            vectortype   = True
        if param['type'] == PySlo.type.color:
            vectortype   = True
        if vectortype:
            sys.stdout.write("[%g %g %g]" % (value[0], value[1], value[2]))
        
        if param['type'] == PySlo.type.matrix:
            sys.stdout.write("[%g " % value[0])
            for ii in xrange(1, 15):
                sys.stdout.write("%g " % value[ii])
            sys.stdout.write("%g]" % value[15])
        
        if param['type'] == PySlo.type.string:
            sys.stdout.write("\"%s\"" % value)
        
        if param['type'] == PySlo.type.shader:
            sys.stdout.write("\"(nil)\"")
        
        if i != len(param['default']) - 1:
            sys.stdout.write(", ")
    
    if PySlo.isArray(x):
        sys.stdout.write("}")
    
    sys.stdout.write("\n")
    
sys.stdout.write("\n")

PySlo.endShader()