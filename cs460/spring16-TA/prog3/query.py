import pymysql
import os
import sys

station_map = {
    1: 'Tucson', 2: 'Yuma Valley', 4: 'Safford',
    5: 'Coolidge', 6: 'Maricopa', 7: 'Aguila', 8: 'Parker',
    9: 'Bonita', 12: 'Phx. Greenway',
    14: 'Yuma N. Gila', 15: 'Phx. Encanto',
    19: 'Paloma', 20: 'Mohave',
    22: 'Queen Creek', 23: 'Harquahala', 24: 'Roll',
    26: 'Buckeye', 27: 'Desert Ridge', 28: 'Mohave #2',
    29: 'Mesa', 32: 'Payson', 33: 'Bowie',
    35: 'Parker-2', 36: 'Yuma South', 37: 'San Simon',
    38: 'Sahuarita',
}

def query1(cur):
    output = {}
    for k,v in station_map.items():
        sql = """select count(*) from azmet where station='{}'
        and precipitation_total>0""".format(v)
        cur.execute(sql)
        rows = cur.fetchall()
        output[v] = rows[0][0]

    print('Query #1\n')
    print('station'.ljust(20), 'days of precipitation')
    print('-' * 42)
    for station in sorted(output, key=output.get, reverse=True):
        print(station.ljust(20), output[station])


def query2(cur):
    print('\nQuery #2\n')
    print('Station')
    print('-------')
    for k,v in station_map.items():
        print(v)

    station_name = input('Pick a station from above: ')
    max_temp_count = 0

    for day in range(1, 366):
        air_temp_max_map = {}
        query = """select station, air_temp_max from azmet where
        day_of_year={}""".format(day)
        cur.execute(query)
        rows = cur.fetchall()
        for row in rows:
            if row[1] is not None:
                air_temp_max_map[row[0]] = float(row[1])

        max_temp = max(air_temp_max_map.values())
        # print(max_temp)
        try:
            if air_temp_max_map[station_name] == max_temp:
                max_temp_count += 1
                # print(station_name, air_temp_max_map[station_name])
        except KeyError:
            pass

    print('station'.ljust(15),
          '# of days with the highest max air temperature across all of the stations')
    print('-' * 90)
    print(station_name.ljust(15), max_temp_count)


def query3(cur):
    print('\nQuery #3\n')

    month = input('Enter a month(in number): ')

    print('station'.ljust(15), 'day of the month'.ljust(20),
          'highest mean wind speed recorded')
    print('-' * 90)

    # for k,v in station_map.items():
    for v in sorted(station_map.values()):
        highest_mean_wind_speed = 0
        day_of_month = []
        sql = """select day_of_month, wind_speed_mean from azmet where station='{}'
        and month={}""".format(v, month)
        cur.execute(sql)
        rows = cur.fetchall()
        for row in rows:
            if row[1] is not None:
                if float(row[1]) > highest_mean_wind_speed:
                    highest_mean_wind_speed = float(row[1])
                    day_of_month[:] = [] # empty the list
                    day_of_month.append(int(row[0]))
                elif float(row[1]) == highest_mean_wind_speed:
                    day_of_month.append(int(row[0]))

        for day in day_of_month:
            print(v.ljust(15), str(day).ljust(20), highest_mean_wind_speed)

        # print(v.ljust(15), str(day_of_month).ljust(20), highest_mean_wind_speed)



if __name__ == '__main__':
    conn = pymysql.connect(host='127.0.0.1',
                           user='root',
                           passwd='',
                           db='azmet')
    cur = conn.cursor()
    query1(cur)
    #query2(cur)
    query3(cur)
