#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

bool debug = false;

queue<char> stringToQueue(string str){
  queue<char> newQueue;
  for(char s : str){
    newQueue.push(s);
  }
  return newQueue;
}

char pop_get(queue<char>* queue){
  char value = queue->front();
  queue->pop();
  return queue->front();
}

string getNextTag() {
  char start = 'p';

  while (start != '<') {
    cin >> start;
    if (debug) {
    }
  }
  string tag;
  start = 'a';
  while (isalpha(start) || start == '/') {
    cin >> start;
    tag.push_back(start);
  }

  if (debug) {
    cout << "Read tag: " << tag;
  }
  return tag;
}

string getNextValue() {
  char temp;
  while (temp != '\"') {
    cin >> temp;
  }
  string word;
  cin >> temp;
  word.push_back(temp);
  while (temp != '\"') {
    cin >> temp;
    word.push_back(temp);
  }
  word.pop_back();
  return word;
}

string getNextName() {
  char temp;
  string word;
  while (temp != '=') {

    cin >> temp;
    if (temp == '>') {
      return "";
    }
    word.push_back(temp);
  }
  word.pop_back();
  return word;
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
  }
};

class HrmlObjectFactory {
  list<HrmlObject> objects;

public:
  HrmlObject *createHrml() {
    objects.emplace_back();
    return &objects.back();
  }
};

void getTree(HrmlObject *object) {
  cout << object->tag << endl;
  for (HrmlObject *child : object->children) {
    getTree(child);
  }
};

HrmlObject *findObjects(HrmlObject *headr, queue<string> tgs) {
  bool childFound = false;

  for (auto *child : headr->children) {
    if (child->tag == tgs.front()) {
      tgs.pop();
      if (tgs.size() <= 0) {
        return child;
      } else {
        return findObjects(child, tgs);
      }
    }
  }
  return nullptr;
}

int main() {
  /* Enter your code here. Read input from STDIN. Print output to STDOUT */
  string line;
  
  HrmlObjectFactory factory;
  HrmlObject *head = factory.createHrml();
  head->tag = "head";
  if (debug) {
    cout << "starting function" << endl;
  }
  int n;
  int q;
  stack<HrmlObject *> stack;
  stack.push(head);
  scanf("%d %d", &n, &q);
  if (debug) {
    cout << "scanned: " << n << " " << q << endl;
  }
  int o;
  vector<HrmlObject *> items;
  string tag;
  string nextWord;
  int counter;
  char temp;
  if (debug) {
    cout << "starting main loop" << endl;
  }
  int open_index = 0;
  for (int i = 0; i < n; i++) {
    getline(cin, line);
    tag = getNextTag();
    if (debug) {
      cout << "starting read tag loop" << endl;
    }
    if (tag[0] != '/') {
      items.push_back(factory.createHrml());
      items[open_index]->tag = tag;
      counter = 0;
      nextWord = getNextName();
      while (nextWord[0] != '>') { // While there are more names
        items[open_index]->attributes.emplace_back();
        items[open_index]->attributes[counter].name = nextWord;

        nextWord = getNextValue();
        items[open_index]->attributes[counter].value = nextWord;
        nextWord = getNextName();
        if (nextWord.length() == 0) {
          break;
        }
        counter++;
      }
      stack.push(items[open_index]);
      open_index++;

    } else {
      HrmlObject *child = stack.top();
      stack.pop();
      HrmlObject *parent = stack.top();
      parent->children.push_back(child);
      if (debug) {
        getTree(head);
      }
    }
  };
  // parsing queries
  for (int i = 0; i < q; i++) {
    bool error;
    string currentTag;
    queue<string> tags;
    // get tags
    temp = 'b';
    while (temp != '~') {
      cin >> temp;

      if (temp == '.' || temp == '~') {
        tags.push(currentTag);
        currentTag = "";
      } else if (temp == '>') {
        continue;
      } else if (temp == '\n') {
        error = true;
        break;
      } else {
        currentTag.push_back(temp);
      }
    }
    if (error) {
      cout << "Not Found!" << endl;
      break;
    }

    string nextTag = "";
    // get attribute names
    // at this point I realized that you can getline *facepalm*
    string line;
    temp = 'b';
    // currentAttribute should contain potatotag1
    
    string attribute = line;
    HrmlObject *object;
    object = findObjects(head, tags);
    if (object->tag == "") {
      cout << "Not Found!" << endl;
    }
    if (debug) {
      cout << object->attributes.capacity();
    }
    bool valueFound = false;
    // find value
    for (auto attr : object->attributes) {
      if (attr.name == attribute) {
        cout << attr.value << endl;
        valueFound = true;
      }
    }
    if (!valueFound) {
      cout << "Not Found!" << endl;
    }
  }
}
