/**
 *  ofxCsv.cpp
 *  Inspired and based on Ben Fry's [table class](http://benfry.com/writing/map/Table.pde)
 *
 *  
 *  The MIT License
 *
 *  Copyright (c) 2011-2014 Paul Vollmer, http://www.wng.cc
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 *  
 *  @testet_oF          0071
 *  @testet_plattform   MacOs 10.6+
 *                      ??? Win
 *                      ??? Linux
 *  @dependencies       
 *  @modified           2012.06.28
 *  @version            0.1.3
 */

#include "ofxCsv.h"





namespace wng {
	
	/**
	 * A Constructor, usually called to initialize and start the class.
	 */
	ofxCsv::ofxCsv(){
		filePath = "";
		// set the default seperator value
		fileSeparator = ",";
    fileComments = "#";
		numRows = 0;
    numCols = 0;
	}

  void ofxCsv::ParseCSV(ifstream& csvSource, vector<vector<string> >& lines) {
    csvSource.seekg (0, csvSource.beg);
    ParseCSVString(csvSource, lines);
  }

  void ofxCsv::ParseCSVString(istream& csvSource, vector<vector<string> >& lines)
  {
    bool inQuote(false);
    bool newLine(false);
    string field;
    lines.clear();
    vector<string> line;

    //std::istream_iterator<char> eos;
    //std::istream_iterator<char> aChar(csvSource);

    char aChar;
    while (csvSource.get(aChar))
    {
      switch (aChar)
      {
        case '"':
          newLine = false;
          inQuote = !inQuote;
          break;
          
        case ',':
          newLine = false;
          if (inQuote == true)
          {
            field += aChar;
          }
          else
          {
            line.push_back(field);
            field.clear();
          }
          break;
          
        case '\n':
        case '\r':
          if (inQuote == true)
          {
            field += aChar;
          }
          else
          {
            if (newLine == false)
            {
              line.push_back(field);
              lines.push_back(line);
              ofLog(OF_LOG_VERBOSE, "Parsed %i fields", line.size());

              field.clear();
              line.clear();
              newLine = true;
            }
          }
          break;
          
        default:
          newLine = false;
          field += aChar;
          break;
      }
      
      aChar++;
    }
    
    if (field.size())
      line.push_back(field);
    
    if (line.size())
      lines.push_back(line);
    
    ofLog(OF_LOG_VERBOSE, "Parsed %i rows", lines.size());
    
    for (int r=0; r < lines.size(); r++) {
      ofLog(OF_LOG_VERBOSE, "Line %i", r);
      stringstream message;
      message << "ROW " << ofToString(r) << ":\t";
      for(int l=0; l < lines[r].size(); l++) {
        string field = lines[r][l];
        message << " Field "<< ofToString(l) << " [" << field << "]";
      }
      ofLog(OF_LOG_VERBOSE, message.str() );
    }

    // Save the Number of Rows & Cols.
    numRows = data.size();
    numCols = numRows > 0 ? data[0].size() : 0;

  }
	
	
	/**
	 * Load a CSV File.
	 *
	 * @param path
	 *        Set the File path.
	 * @param separator
	 *        Set the Separator to split CSV File.
	 * @param comments
	 *        Set the Comments sign.
	 */
	void ofxCsv::loadFile(string path, string separator, string comments){
    #ifdef OFXCSV_LOG
        ofLog() << "[ofxCsv] loadFile";
        ofLog() << "         filePath: " << filePath;
        ofLog() << "         fileSeparator: " << fileSeparator;
        ofLog() << "         fileComments: " << fileComments;
    #endif
    
		// Save Filepath, Separator and Comments to variables.
		this->filePath = path;
		this->fileSeparator = separator;
		this->fileComments = comments;
		#ifdef OFXCSV_LOG
			ofLog() << "[ofxCsv] loadFile";
			ofLog() << "         filePath: " << filePath;
			ofLog() << "         fileSeparator: " << fileSeparator;
			ofLog() << "         fileComments: " << fileComments;
		#endif
		
		// Declare a File Stream.
    ifstream fileIn;
	
		// Open your text File:
		fileIn.open(path.c_str());
	
		// Check if File is open.
		if(fileIn.is_open()) {

      ParseCSV(fileIn, data);
		
		} else {
      // If File cannot opening, print a message to console.
			cerr << "[ofxCsv] Error opening " << path << ".\n";
		}
	
	}
    
    void ofxCsv::setData( vector<vector<string> > data)
    {
        this->data = data;
        numRows = data.size();
    }
	
	
	void ofxCsv::clear()
    {
        for( int i = 0; i < data.size(); i++ )
        {
            data[i].clear();
        }
        data.clear();
		numRows = 0;
    }
	
	
	/**
	 * Load a CSV File.
	 * The default Comment sign is "#".
	 *
	 * @param path
	 *        Set the file path.
	 * @param separator
	 *        Set the Separator to split CSV file.
	 */
	void ofxCsv::loadFile(string path, string separator){
		
		loadFile(path, separator, "#");
	
	}
	
	
	
	
	
	/**
	 * Load a CSV File.
	 * The default Separator is ",".
	 * The default Comment sign is "#".
	 *
	 * @param path
	 *        Set the file path.
	 */
	void ofxCsv::loadFile(string path){
		
		loadFile(path, ",", "#");
	
	}
	
	
	
	
	
	/**
	 * saveFile
	 *
	 * @param path
	 *        Set the file path.
	 * @param separator
	 *        Set the Separator to split CSV file.
	 * @param comments
	 *        Set the Comments sign.
	 */
	void ofxCsv::saveFile(string path, string separator, string comments){
		
		createFile(path);
		
		ofstream myfile;
		myfile.open(path.c_str());
		if(myfile.is_open()){
			// Write data to file.
			for(int i=0; i<numRows; i++){
				for(int j=0; j<data[i].size(); j++){
					
					myfile << data[i][j];
					
					if(j==(data[i].size()-1)){
						myfile << "\n";
					} else {
						myfile << separator;
					}
				}
			}
			myfile.close();
//			cout << "Open file" << endl;
		} else {
//			cout << "Unable to open file    " << endl;
		}
	
	}
	
	
	
	
	
	/**
	 * saveFile
	 *
	 * @param path
	 *        Set the file path.
	 * @param separator
	 *        Set the Separator to split Csv file.
	 */
	void ofxCsv::saveFile(string path, string separator) {
	
		//createFile(path);
		saveFile(path, separator, fileComments);
	
	}
	
	
	
	
	
	/**
	 * saveFile
	 *
	 * @param path
	 *        Set the file path.
	 */
	void ofxCsv::saveFile(string path) {
	
		//createFile(path);
		saveFile(path, fileSeparator, fileComments);
	
	}
	
	
	
	
	
	/**
	 * Save file.
	 */
	void ofxCsv::saveFile() {
		
		saveFile(filePath, fileSeparator, fileComments);
	
	}
	
	
	
	
	
	/**
	 * createFile
	 *
	 * @param path
	 *        Set the File Path.
	 */
	void ofxCsv::createFile(string path){
		
		FILE * pFile;
		pFile = fopen (path.c_str(),"w");
		
		if (pFile!=NULL) {
			//fputs ("fopen example",pFile);
			fclose (pFile);
		}
		
	}
	
	
	
	
	
	/**
	 * loadFromString
	 *
	 * @param s
	 *        String Input.
	 * @param separator
	 *        Set the Separator to split CSV string.
	 */
	vector<string> ofxCsv::getFromString(string csv, string separator){
	
		vector<string> cols = ofSplitString(csv, separator);
		return cols;
	
	}
	
	
	
	
	
	/**
	 * loadFromString
	 *
	 * @param s
	 *        String Input.
	 */
	vector<string> ofxCsv::getFromString(string csv){
		
		return getFromString(csv, ",");
	
	}
	
	
	
	
	
	/**
	 * Get the Integer of a specific row and column.
	 *
	 * @param row
	 *        row number
	 * @param col
	 *        column number
	 * @return integer
	 */
	int ofxCsv::getInt(int row, int col){
		return ofToInt(data[row][col]);//temp;
	}
	
	
	/**
	 * Get the Float of a specific row and column.
	 *
	 * @param row
	 *        row number
	 * @param col
	 *        column number
	 * @return float
	 */
	float ofxCsv::getFloat(int row, int col){
		allocateData(row, col);
		return ofToFloat(data[row][col]);//temp;
	
	}
	
	
	
	
	
	/**
	 * Get the String of a specific row and column.
	 *
	 * @param row
	 *        row number
	 * @param col
	 *        column number
	 * @return float
	 */
	string ofxCsv::getString(int row, int col){
		allocateData(row, col);
		return data[row][col];
	}
	
	
	
	
	
	/**
	 * Get the Boolean of a specific row and column.
	 *
	 * @param row
	 *        row number
	 * @param col
	 *        column number
	 * @return bool
	 */
	bool ofxCsv::getBool(int row, int col){
		allocateData(row, col);
		return ofToBool(data[row][col]);
	
	}
	
	
	
	
	
	/**
	 * Set a specific Integer to a new value.
	 *
	 * @param row
	 *        row number
	 * @param col
	 *        column number
	 * @param what
	 *        new Integer
	 */
	void ofxCsv::setInt(int row, int col, int what){
		allocateData(row, col);
		data[row][col] = ofToString(what);
	
	}
	
	
	
	
	
	/**
	 * Set a specific Float to a new value.
	 *
	 * @param row
	 *        row number
	 * @param col
	 *        column number
	 * @param what
	 *        new row Float
	 */
	void ofxCsv::setFloat(int row, int col, float what){
		allocateData(row, col);
		data[row][col] = ofToString(what);
	
	}
	
	
	
	
	
	/**
	 * Set a specific String to a new value.
	 *
	 * @param row
	 *        row number
	 * @param col
	 *        column number
	 * @param what
	 *        new row String
	 */
	void ofxCsv::setString(int row, int col, string what){
		allocateData(row, col);
		data[row][col] = ofToString(what);
	
	}
	
	
	
	
	
	/**
	 * setBool
	 * set a specific Boolean to a new value.
	 *
	 * @param row
	 *        row number
	 * @param col
	 *        column number
	 * @param what
	 *        new row Boolean
	 */
	void ofxCsv::setBool(int row, int col, bool what){
		allocateData(row, col);
		data[row][col] = ofToString(what);
	
	}
	
    void ofxCsv::allocateData(int row, int col)
    {
        if ( data.size() <= row)
        {
            data.push_back(vector<string>());
            numRows = data.size();
        }
        if ( data[ row ].size() <= col ) data[ row ].push_back("");
    }
}
