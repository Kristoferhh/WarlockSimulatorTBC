import os


def get_icon_name_from_string(string: str):
    return string[string.find('IconName') + len('IconName: "'):len(string) - 3]

def assert_icon_image_existence_in_file(file_path: str):
    with open(file_path) as f:
        for line in f:
            if 'IconName' in line and not 'IconName: string' in line:
                icon_name = get_icon_name_from_string(line)
                assert os.path.isfile(f'./public/img/{icon_name}.jpg'), f'Image for icon `{icon_name}` not found in ./public/img/'

def auras():
    assert_icon_image_existence_in_file('./src/data/Auras.tsx')

def gems():
    assert_icon_image_existence_in_file('./src/data/Gems.tsx')

def items():
    assert_icon_image_existence_in_file('./src/data/Items.tsx')

def sockets():
    assert_icon_image_existence_in_file('./src/data/Sockets.tsx')

def spells():
    assert_icon_image_existence_in_file('./src/data/Spells.tsx')

def talents():
    assert_icon_image_existence_in_file('./src/data/Talents.tsx')

if __name__ == "__main__":
    auras()
    gems()
    items()
    sockets()
    spells()
    talents()
