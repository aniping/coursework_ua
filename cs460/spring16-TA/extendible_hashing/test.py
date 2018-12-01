import csv

spm_file = open('./spmcat.dat')
spm_csv = csv.reader(spm_file, delimiter=' ')

spm_sum = 0
counter = 0
key = '2630210'
key_matches = 0
for row in spm_csv:
    spm_sum += int(row[0])
    counter += 1
    if row[0].startswith(key):
        key_matches += 1

print('Sum: {}'.format(spm_sum))
print('Mean: {}'.format(spm_sum/counter))
print('# of matches found(key: {}): {}'.format(key, key_matches))
