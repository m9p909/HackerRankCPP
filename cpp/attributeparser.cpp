#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <string>
#include <vector>
using namespace std;

bool debug = true;

queue<char> stringToQueue(string str) {
  queue<char> newQueue;
  for (char s : str) {
    newQueue.push(s);
  }
  return newQueue;
}

class Attribute {
public:
  string name;
  string value;
};

class HrmlObject {
public:
  string tag;
  vector<Attribute> attributes;
  list<HrmlObject *> children;
  /**
   * Gets the attribute with the following name
   */
  string getAttribute(string tag) {
    for (Attribute attr : attributes) {
      if (attr.name == tag) {
        return attr.value;
      }
    }
    return "";
  }
  static void getTree(HrmlObject *object) {
    cout << object->tag << endl;
    for (HrmlObject *child : object->children) {
      getTree(child);
    }
  };
};

class HrmlObjectFactory {
  list<HrmlObject> objects;

public:
  HrmlObject *createHrml() {
    objects.emplace_back();
    return &objects.back();
  }
};

HrmlObject *findObject(HrmlObject *headr, queue<string> tgs) {
  for (auto *child : headr->children) {
    if (child->tag == tgs.front()) {
      tgs.pop();
      if (tgs.size() <= 0) {
        return child;
      } else {
        return findObject(child, tgs);
      }
    }
  }
  return nullptr;
}

enum STATUS { TAG, NAME, VALUE };

int main() {
  /* Enter your code here. Read input from STDIN. Print output to STDOUT */
  vector<string> hrmlLines;
  int n, q;
  cin >> n >> q;
  string line;
  HrmlObjectFactory factory;
  HrmlObject *head = factory.createHrml();
  stack<HrmlObject *> stack;
  stack.push(head);
  HrmlObject *currentObj;

  for (int i = 0; i <= n; i++) {
    getline(cin, line);
    if (line[1] != '/') {
      STATUS status = TAG;
      string tag;
      string name;
      string value;
      Attribute cAttr;
      bool inQuotes = false;
      for (char s : line) {

        if (status == TAG) {
          // get tag
          switch (s) {
          case '<':
            continue;
          case '>':
          case ' ' :
            status = NAME;
            currentObj = factory.createHrml();
            currentObj->tag = tag;

            break;
          default:
            tag.push_back(s);
          }
        } else if (status == NAME) {
          // get Name
          switch (s) {
          case '\"':
            inQuotes = !inQuotes;
            break;
          case ' ':
            break;
          case '=':
            cAttr.name = name;
            status = VALUE;
            break;
          default:
            name.push_back(s);
          }
          // get value
        } else if (status == VALUE) {
          switch (s) {
          case '\"':
            inQuotes = !inQuotes;
            break;
          case ' ':
            if (!inQuotes && value.length() > 0) {
              // save and reset attribute
              cAttr.value = value;
              currentObj->attributes.push_back(cAttr);
              cAttr.name = "";
              cAttr.value = "";
              name = "";
              value = "";
              status = NAME;
            }
            break;

          default:
            if (inQuotes) {
              value.push_back(s);
            } else {
              // save and reset attributes
              cAttr.value = value;
              currentObj->attributes.push_back(cAttr);
              cAttr.name = "";
              cAttr.value = "";
              name = "";
              value = "";
              status = NAME;

              break;
            }
          }
        }
      }
      if (line.length() > 0) {
        stack.push(currentObj);
      }

    } else {
      HrmlObject *child = stack.top();
      stack.pop();
      stack.top()->children.push_back(child);
    }
  }
  enum QSTATUS { TAGS, ATTR };
  for (int i = 0; i < q; i++) {
    getline(cin, line);
    string tag;
    queue<string> tags;
    string attr;
    QSTATUS status = TAGS;
    for (char s : line) {
      switch (status) {
      case TAGS:
        switch (s) {
        case '.':
          tags.push(tag);
          tag = "";
          break;
        case '~':
          tags.push(tag);
          tag = "";
          status = ATTR;
          break;
        default:
          tag.push_back(s);
        }
        break;
      case ATTR:
        attr.push_back(s);
        break;
      }
    }
    string output = "";
    HrmlObject *qResult = findObject(head, tags);
    if (qResult) {
      output = qResult->getAttribute(attr);
    }
    if (output.length() > 0) {
      cout << output << endl;
    } else {
      cout << "Not Found!" << endl;
    }
  }
}