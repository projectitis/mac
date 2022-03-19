from pathlib import Path

###
### ADD COMMENT BLOCKS TO SOURCE FILES
###

commentblock = """/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/"""

# Headers
for path in Path('./../src').rglob('*.h'):
    # Ignore Mac.h
    if path.name == 'Mac.h': continue

    # Get contents
    contents = path.read_text().split('\n')
    new_contents = []

    # Remove comment block from head of file
    print('header: ' + path.name)
    removing = False
    for line in contents:
        cleanline = line.strip()
        if removing:
            if cleanline.endswith('*/'):


    # Remove comment block that preceeds namespace

    # Add new comment block preceeding namespace
    break

# Source
for path in Path('./../src').rglob('*.cpp'):
    print('source: ' + path.name)

# If called directly (ignored if run by another script)
if __name__ == '__main__':
	# Wait before exit for user to read messages
	print()
	input("Press enter to close")
