
export PYTHONPATH="/Users/malcolm/Documents/dev/pyslo/build/dist/lib/python"

shaderdl --embed-source test_rsl1.sl
shaderdl --embed-source test_rsl2.sl
shaderdl --embed-source test_rsl2_big.sl

python test.py test_rsl1.sdl
python test.py test_rsl2.sdl
python test.py test_rsl2_big.sdl

rm test_rsl1.sdl test_rsl2.sdl test_rsl2_big.sdl
