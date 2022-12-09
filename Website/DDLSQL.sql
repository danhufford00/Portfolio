DROP TABLE OrderContains;
DROP TABLE Inventory;
DROP TABLE Ship;
DROP TABLE EmpLogin;
DROP TABLE OrderPart;

/**
 * id       : unique id order number (PRIMARY KEY)
 * Name     : name of customer
 * shipAddr : shipping address of order
 * email    : customers email address
 * CCNum    : credit card number
 * CCExp    : credit card experation date
 */
CREATE TABLE OrderPart(
    id        INT NOT NULL AUTO_INCREMENT,
    name      VARCHAR(30) NOT NULL,
    shipAddr  VARCHAR(60) NOT NULL,
    email     VARCHAR(30) NOT NULL,
    CCNum     VARCHAR(16) NOT NULL,
    CCExp     DATE     NOT NULL,
    isComplete INT     NOT NULL DEFAULT 0,
    weight     INT     NOT NULL DEFAULT 0,
    dateSub    DATE    NOT NULL,
    total      DECIMAL(12,2) NOT NULL DEFAULT 0,
    
    PRIMARY KEY (id)
);

/**
 * partNum  : part numer in invetory         (PRIMARY KEY)
 *            this needs to be the same as
 *            in the legacy DB but we can't
 *            reference it as a foriegn key
 *            since it is in another DB        
 * qty      : quanity of that part in invetory
 */
CREATE TABLE Inventory(
    partNum INT NOT NULL,
    qty  INT,

    PRIMARY KEY (partNum)
);

/**
 * bracket  : lower bound of shipping bracket    (PRIMARY KEY)     
 * price    : shipping price of that bracket
 */
CREATE TABLE Ship(
    bracket   INT PRIMARY KEY,
    price     FLOAT
);

/**
 * orderId  : Order id that has this part ordered(PRIMARY KEY)(FORIEGN KEY)     
 * partNum  : part number that is being ordered  (PRIMARY KEY)
 * qty      : quanity of this part number in the order
 */
CREATE TABLE OrderContains(
    orderId    INT NOT NULL,
    partNum    INT NOT NULL,
    qty        INT,

    PRIMARY KEY (orderId, partNum),
    FOREIGN KEY (orderId) REFERENCES OrderPart(id)
);

CREATE TABLE EmpLogin(
    username   CHAR(15) PRIMARY KEY,
    password   CHAR(10) NOT NULL
);
