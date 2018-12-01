DROP TABLE IF EXISTS Booking, Room, Hotel, Guest;

CREATE TABLE Hotel (
       hotelNo int NOT NULL,
       hotelName varchar(100),
       city varchar(100),
       PRIMARY KEY (hotelNo)
);

CREATE TABLE Room (
       roomNo int NOT NULL,
       hotelNo int,
       roomType char(1),
       price int,
       PRIMARY KEY (roomNo, hotelNo),
       FOREIGN KEY (hotelNo) REFERENCES Hotel (hotelNo)
);

CREATE TABLE Guest (
       guestNo int NOT NULL,
       guestName varchar(100),
       guestAddress varchar(100),
       PRIMARY KEY (guestNo)
);

CREATE TABLE Booking (
       hotelNo int,
       guestNo int,
       dateFrom date NOT NULL,
       dateTo date NOT NULL,
       roomNo int,
       PRIMARY KEY (hotelNo, guestNo, dateFrom),
       FOREIGN KEY (hotelNo) REFERENCES Hotel (hotelNo),
       FOREIGN KEY (guestNo) REFERENCES Guest (guestNo)
);

commit;
