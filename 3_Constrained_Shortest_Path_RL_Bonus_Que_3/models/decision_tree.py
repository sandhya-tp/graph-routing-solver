from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split



def train_decision_tree(X, y, test_size, val_size, random_state):
    X_temp, X_test, y_temp, y_test = train_test_split(X, y, test_size=test_size, random_state=random_state)

    X_train, X_val, y_train, y_val = train_test_split(X_temp, y_temp, test_size=val_size, random_state=random_state)

    clf = DecisionTreeClassifier()
    clf.fit(X_train, y_train)
    accuracy = clf.score(X_test, y_test)
    #print("Test Accuracy of decision tree:", accuracy)

    return clf, X_train, X_test, y_train, y_test, X_val, y_val


def train_random_forest(X, y, test_size, val_size, random_state):
    X_temp, X_test, y_temp, y_test = train_test_split(X, y, test_size=test_size, random_state=random_state)

    X_train, X_val, y_train, y_val = train_test_split(X_temp, y_temp,
        test_size=val_size, random_state=random_state)
    clf = RandomForestClassifier(n_estimators=100, random_state=random_state, n_jobs=-1)
    
    clf.fit(X_train, y_train)

    

    return clf, X_train, X_val, y_train, y_val, X_test, y_test


