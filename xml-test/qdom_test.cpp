
#include <qdom.h>
#include <iostream>
#include <qfile.h>

using namespace std;

int main() {
	QDomDocument doc(  "mydocument" );
	QFile file(  "mydocument.xml" );
	if (  !file.open(  IO_ReadOnly ) )
		return 2;
	if (  !doc.setContent(  &file ) ) {
		file.close();
		return 1;
	}
	file.close();

	// print out the element names of all elements that are direct children
	// of the outermost element.
	QDomElement docElem = doc.documentElement();
	cout << docElem.tagName() <<endl;

	QDomNode n = docElem.firstChild();
	while (  !n.isNull() ) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if (  !e.isNull() ) {
			cout << e.tagName() << endl; // the node really is an element.
		}
		n = n.nextSibling();
	}

	return 0;
}

