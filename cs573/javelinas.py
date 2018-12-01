import random

numOfJavelinas = 300
lenOfWall = 600

javelinas = []

class Javelina( object ):
    def __init__( self ):
        self.position = 0
        self.direction = ''
        self.ID = 0
        self.endOfWall = False
        self.inTransit = False

    def __cmp__( self, other ):
        return cmp( self.position, other.position )

    def __str__( self ):
        return str( self.position ) + ',' + self.direction

    def setDirection( self, direction ):
        self.direction = direction

    def reverseDirection( self ):
        if self.direction == 'l':
            self.direction = 'r'
        elif self.direction == 'r':
            self.direction = 'l'

    def toLeft( self ):
        if self.direction == 'l':
            return True
        else:
            return False

    def toRight( self ):
        if self.direction == 'r':
            return True
        else:
            return False

    def isDone( self ):
        return self.endOfWall

    def isInTransite( self ):
        return self.inTransit

    def moveOneFoot( self ):
        if self.toLeft() == True:
            self.position = self.position - 1
        elif self.toRight() == True:
            self.position = self.position + 1

def dropJavelinas():
    """Drop javelinas in random places along the wall."""
    nums = random.sample( range(0, lenOfWall+1), numOfJavelinas )
    ID = 1
    for num in nums:
        javelina = Javelina()
        javelina.position = num
        if ( random.getrandbits(1) ):
            javelina.setDirection( 'l' )
        else:
            javelina.setDirection( 'r' )

        javelina.ID = ID
        ID += 1
        javelinas.append( javelina )

def checkConflict( javelina, index ):
    """Check if the 'javelina' would bump into another one on its way."""
    if javelina.toLeft() == True: # Moving to left
        next_index = index - 1
    elif javelina.toRight() == True: # Moving to right
        next_index = index + 1

    # check index range
    if next_index < 0 or next_index >= numOfJavelinas:
        return -1 # no conflict

    javelina2 = javelinas[ next_index ]
    if javelina2.position == 0 or javelina2.position == lenOfWall:
        return -1

    distance = abs( javelina.position - javelina2.position )
    if distance == 1 and javelina.direction != javelina2.direction:
        print 'javelina', javelina.ID, 'at position', javelina.position, \
            'conflicts with', 'javelina', javelina2.ID, 'at position', javelina2.position
        return next_index # a conflict detected
    else:
        return -1 # no conflict

def clearWall():
    timer = 0
    end_wall_counter = 0
    while ( end_wall_counter < numOfJavelinas ):
        timer += 1
        for index, javelina in enumerate(javelinas):
            # if this javelina has reached to the end of wall,
            # no need to go further.
            if javelina.isDone() == True:
                continue 
            
            if javelina.position == 0 or javelina.position == lenOfWall:
                print 'Javelina', javelina.ID, 'is done'
                end_wall_counter += 1
                javelina.endOfWall = True
            else:
                neighbor_index = checkConflict( javelina, index )
                if neighbor_index != -1:
                    neighbor = javelinas[ neighbor_index ]
                    javelina.inTransit = True
                    neighbor.inTransit = True
                else:
                    javelina.moveOneFoot()

        for javelina in javelinas:
            if javelina.inTransit == True:
                javelina.inTransit = False
                javelina.reverseDirection()

        index = 0
        for javelina in javelinas:
            if index >= numOfJavelinas or (index+1) >= numOfJavelinas:
                break
                
            if javelina.position == javelinas[ index+1 ].position:
                javelina.reverseDirection()
                javelinas[ index+1 ].reverseDirection()
                index += 2
            else:
                index += 1

        javelinas.sort()

    print 'Total time (sec):', timer
    return timer

def movingInOneTimeStep():
    pass

def main():
    dropJavelinas()
    javelinas.sort()
    #printStatus()
    #print len(javelinas)
    time = clearWall()
    for javelina in javelinas:
        print javelina

    print 'Total time (sec):', time

if __name__ == '__main__':
    main()
