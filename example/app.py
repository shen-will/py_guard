
#app entry
import py_guard
import boot
py_guard.set_encode(boot.encode)
py_guard.set_decode(boot.decode)



import test
h = test.show_info()
h()

test.show_game()