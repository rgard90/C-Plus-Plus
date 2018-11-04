# Speak Like a Document Lab
This tutorial will lead you through a fun experience with using maps to implement complex learning functions.  

Machine learning has become increasingly popular in Computer Science.  A training set is used to set up the probability of an event occurring given a set of inputs.  In this lab you will train a map on the words from a document and then produce text that has the same distribution of words as the original document.

In order to complete this lab, you will need to become familiar with maps.  Maps are like arrays or vectors except that they can be indexed by any data type.  So you can index into a Map using a string or almost anything else.

If you are reading english text, the context of words in a phrase can be represented by a deque.  If you use this as the key to a Map, then you can create a vector of words that follow that phrases.  

Once you have trained this map from a deque to a vector of next words, you can generate text that sounds like the document by choosing a random key from the Map, then follow that key to a random next word.  Then look up the deque containing generated words to find the next word in your generated text.

To start with, you should make sure you can read in the words from a book and place them in a vector of strings.  We did this in the last learning activity, so make sure that code is working.  If you haven't got this working, go through the [sets learning activity](https://github.com/BYUCS235/sets).

First lets try a map of strings to strings.  As we go through strings in the vector, we will insert the next string into the map as the value for the current string.  So, for the phrase "having been born of goodly parents", the map for key "having" would be "been", or wordmap["having"]="been".  Insert code to create this map.  Then read through the vector of strings and insert them into the map.  We can print out the map to make sure everything looks OK.
```c++
    map <string, string> wordmap;
    string last="";
    for(auto s:tokens) {
        wordmap[last]=s;
        last = s;
    }
    for (map<string,string>::iterator it=wordmap.begin(); it!=wordmap.end(); ++it)
        cout << it->first<<' ' << it->second<<endl;
```
The map has learned the context for each word in the document, so we can generate text using this context.  For this example, we will generate 100 words of text.
```c++
    //Now generate text
    string current = "";
    for(int i =0; i < 100; i++) {
        cout << wordmap[current]<<" ";
        current = wordmap[current];
    }
    cout << endl;
 ```
 If you run this code on the 1Nephi.text, you should get something like 
```
I and endure to the last day And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it is Amen And thus it
```
Notice that we get stuck in a loop because we are only keeping one of the next words of context that we learn from the document.  

Lets keep track of all the words that are seen after a word.  Change the map to be a map with strings as keys to vectors of strings for values.  Then push each of the context values into this array as you see words from the document.
```c++
    map <string, vector<string>> wordmap;
    string last="";
    for(auto s:tokens) {
        wordmap[last].push_back(s);
        last = s;
    }
```
And now that we have a vector of strings as context for each word, pick a random entry from the vector for the next word in the generated text.
```c++
    string current = "";
    for(int i =0; i < 100; i++) {
        int ind = rand() % wordmap[current].size();
        cout << wordmap[current][ind]<<" ";
        current = wordmap[current][ind];
    }
    cout << endl;
```
The generated text now sounds a lot more readable and doesn't get stuck in an infinite loop.
```
I looked and people yea even he read he beheld the angel And I have suffered all the flesh and to pass that I might preserve his God of the cross and I have dreamed a wise purpose in God suffer those that As the younger brother Nephi did inquire of the words he spake unto the Lord hath spanned the engravings which was truly chastened because of the dust yea in dominion and also thought to be a man into the wickedness of my bow for the captivity of men to pass that after I was also was prepared 
```
But we are only looking one word ahead in the context and the generated text still doesn't sound very good.  We would really like to use a phrase as a key so that we can learn multiple word context.  So lets change the key for the map to be a deque of strings.  Then we can push words onto one end of the deque and pop words off of the end of the deque and use the deque to capture the context of a phrase.
```c++
    map <deque <string>, vector<string>> wordmap;
    deque <string> state;
    state.push_back("");
    state.push_back("");
    for(auto s:tokens) {
        cout << state[0]<<" "<<state[1]<<"="<<s<<endl;
        wordmap[state].push_back(s);
        state.push_back(s);
        state.pop_front();
    }
```
And now that the map has learned the next word from a context of multiple words, we can generate text using a deque as a key.
```c++
    deque <string> current;
    current.push_back("");
    current.push_back("");

    for(int i =0; i < 100; i++) {
        int ind = rand() % wordmap[current].size();
        cout << wordmap[current][ind]<<" ";
        current.push_back(wordmap[current][ind]);
        current.pop_front();
    }
    cout << endl;
```
The text that is generated now sound much more like english.
```
I Nephi being a natural branch of the seed of thy seed And at that day that they were white and exceedingly fair and white And it came to pass that I beheld your mother Sariah was exceedingly great mist of darkness yea even so much was his mind swallowed up in other words I remembered the words of the first shall be cast off forever And while the angel spake and showed himself unto the word of God after I had spoken unto you the kingdom of God which had been a world And he beheld the whore of 
```
Now you get the general idea, experiment with different ways of capturing even longer context to see if it improves the generated text.  And try your code on other [gutenberg](https://www.gutenberg.org/) texts to tune your code to different inputs.  You will have to determine rubrics for what you think it higher quality generated text.  Be ready to explain what you did to a partner when we pass off the lab.
