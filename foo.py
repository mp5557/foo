# %%
class Loader:
    def __init__(self, fname):
        with open(f'{fname}-index.txt') as f_index:
            lines = f_index.read().splitlines()
            file_size = f_index.seek(0, 2)
        
        key_list, pos_list = zip(*[l.split(' ') for l in lines])
        pos_list = [int(p) for p in pos_list]
        pos_list.append(file_size)

        self.key_list = key_list
        self.key_map = dict()
        for i, key in enumerate(key_list):
            self.key_map[key] = (pos_list[i], pos_list[i + 1] - pos_list[i])
        
        self.f_data = open(f'{fname}-data.bin', 'rb')
    
    def load(self, key):
        pos, size = self.key_map[key]
        self.f_data.seek(pos)
        return self.f_data.read(size)

# %%
loader = Loader('build/foo_db')
print(loader.key_list)

# %%
loader.load('foobar')