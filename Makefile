.PHONY: all clean function

clean:
	git submodule foreach '$(MAKE) clean'

fclean:
	git submodule foreach '$(MAKE) fclean'

checkout:
	git submodule foreach 'git checkout main'
