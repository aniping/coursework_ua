import pymysql
import os
import sys
import csv

station_map = {
    1: 'Tucson', 2: 'Yuma Valley', 3: 'Yuma Mesa', 4: 'Safford',
    5: 'Coolidge', 6: 'Maricopa', 7: 'Aguila', 8: 'Parker',
    9: 'Bonita', 10: 'Citrus Farm', 11: 'Litchfield', 12: 'Phx. Greenway',
    13: 'Marana', 14: 'Yuma N. Gila', 15: 'Phx. Encanto', 16: 'Eloy',
    17: 'Dateland', 18: 'Scottsdale', 19: 'Paloma', 20: 'Mohave',
    21: 'Laveen', 22: 'Queen Creek', 23: 'Harquahala', 24: 'Roll',
    25: 'Ciudad Obregon', 26: 'Buckeye', 27: 'Desert Ridge', 28: 'Mohave #2',
    29: 'Mesa', 30: 'Flagstaff', 31: 'Prescott', 32: 'Payson', 33: 'Bowie',
    34: 'Kansas Settlement', 35: 'Parker-2', 36: 'Yuma South', 37: 'San Simon',
    38: 'Sahuarita',
}

month_map = {
    1: (1, 32), 2: (32, 60), 3: (60, 91), 4: (91, 121), 5: (121, 152),
    6: (152, 182), 7: (182, 213), 8: (213, 244), 9: (244, 274),
    10: (274, 305), 11: (305, 335), 12: (335, 366),
}

def insert(fname, cur, conn):
    snum = int(fname[:2])
    station = station_map[snum]
    print('Inserting records for station-{}: {}'.format(snum, station))
    with open(fname, 'r') as csvfile:
        azmet_reader = csv.reader(csvfile, delimiter=',')
        for row in azmet_reader:
            year = int(row[0])
            day_of_year = int(row[1])
            month = 0
            day_of_month = 0
            for k,v in month_map.items():
                if day_of_year in range(v[0], v[1]):
                    month = k
                    day_of_month = day_of_year - v[0] + 1
                    # print('month: {}, day: {}'.format(month, day_of_month))
                    break

            station_num = int(row[2])
            air_temp_max = float(row[3])
            if air_temp_max == 999:
                air_temp_max = 'NULL'
            air_temp_min = float(row[4])
            air_temp_mean = float(row[5])
            rh_max = float(row[6])
            rh_min = float(row[7])
            rh_mean = float(row[8])
            vpd_mean = float(row[9])
            solar_rad_total = float(row[10])
            precipitation_total = float(row[11])
            soil_temp_4_max = float(row[12])
            soil_temp_4_min = float(row[13])
            soil_temp_4_mean = float(row[14])
            soil_temp_20_max = float(row[15])
            soil_temp_20_min = float(row[16])
            soil_temp_20_mean = float(row[17])
            wind_speed_mean = float(row[18])
            if wind_speed_mean == 999:
                wind_speed_mean = 'NULL'
            wind_vector_magnitude_mean = float(row[19])
            wind_vector_direction_mean = float(row[20])
            wind_direction_stddev = float(row[21])
            max_wind_speed = float(row[22])
            heat_units = float(row[23])
            eto = float(row[24])
            etos = float(row[25])
            actual_vapor_pressure = float(row[26])
            dewpoint = float(row[27])

            sql_insert = """insert into azmet values("{0}",{1},{2},{3},{4},{5},{6},
            {7},{8},{9},{10},{11},{12},{13},{14},{15},{16},{17},{18},{19},{20},{21},
            {22},{23},{24},{25},{26},{27},{28},{29},
            {30})""".format(station,year,day_of_year,month,day_of_month,station_num,
                            air_temp_max,air_temp_min,air_temp_mean,rh_max,rh_min,rh_mean,
                            vpd_mean,solar_rad_total,precipitation_total,soil_temp_4_max,
                            soil_temp_4_min,soil_temp_4_mean,soil_temp_20_max,soil_temp_20_min,
                            soil_temp_20_mean,wind_speed_mean,wind_vector_magnitude_mean,
                            wind_vector_direction_mean,wind_direction_stddev,
                            max_wind_speed,heat_units,eto,etos,actual_vapor_pressure,
                            dewpoint)
            try:
                cur.execute(sql_insert)
            except pymysql.IntegrityError:
                print('Integrity viotion when trying to insert the record:\n{}'.format(sql_insert))


if __name__ == '__main__':
    conn = pymysql.connect(host='127.0.0.1',
                           user='root',
                           passwd='',
                           db='azmet')
    cur = conn.cursor()

    files = os.listdir()
    for f in files:
        if f.endswith('.txt'):
            insert(f, cur, conn)

    conn.commit()
