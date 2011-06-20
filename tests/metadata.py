
import PySlo
PySlo.setPath(".")
PySlo.setShader("test_rsl2_big.sdl")
print PySlo.getNAnnotations()
print PySlo.getNAnnotations()
key = PySlo.getAnnotationKeyById(2)
print key
print PySlo.getAnnotationByKey(key)
print PySlo.getPluginsNames()
print PySlo.getMetaData('author')
print PySlo.getAllMetaData()
