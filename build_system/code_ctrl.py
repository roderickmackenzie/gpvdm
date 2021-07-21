import getpass

def is_rod():
	if getpass.getuser()=="rod":
		return True
	return False
