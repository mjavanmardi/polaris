import cmd

class SM(cmd.Cmd):
	prompt = 'polaris manager: '
	intro = "Scenario manager tool for POLARIS studies"
	def do_greet(self, line):
		print """Plsese select what would you like to do: 
1. Create New Study 			(new_study)
2. Add a run to exisitng study		(add_run)
3. Explre existing study 		(get_study)"""
		def do_EOF(self, line):
			return True

if __name__ == '__main__':
    SM().cmdloop()