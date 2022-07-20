#pragma once
// for opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <msclr\marshal_cppstd.h> 
#include <baseapi.h>
#include <allheaders.h>

namespace form_deneme {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	// for opencv
	using namespace std;
	using namespace cv;
	using namespace System::IO;

	/// <summary>
	/// Summary for MyForm1
	/// </summary>
	public ref class MyForm1 : public System::Windows::Forms::Form
	{
	public:
		MyForm1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::ComboBox^  comboBox1;

	private: System::String^ tempCropFileName = "tempCrop.png";
	private: bool readFlag = false;






	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm1::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(149, 200);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(156, 55);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Start Detect";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm1::button1_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->checkBox1->Location = System::Drawing::Point(34, 20);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(95, 21);
			this->checkBox1->TabIndex = 1;
			this->checkBox1->Text = L"Read Text";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm1::checkBox1_CheckedChanged);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::WindowFrame;
			this->panel1->Controls->Add(this->comboBox1);
			this->panel1->Controls->Add(this->checkBox1);
			this->panel1->Location = System::Drawing::Point(51, 105);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(377, 61);
			this->panel1->TabIndex = 2;
			
			// MyForm1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::InactiveCaptionText;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->ClientSize = System::Drawing::Size(472, 347);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->button1);
			this->DoubleBuffered = true;
			this->Name = L"MyForm1";
			this->Text = L"BeeVision Detector";
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

#pragma region Methods

	private: string convertString(System::String^ managed) {
		string unmanaged = msclr::interop::marshal_as<std::string>(managed);
		return unmanaged;

	}
	private: System::Void TextDetect(System::String^ fileName, System::Windows::Forms::RichTextBox^ richTextBox) {
		richTextBox->Text = "";
		Mat large = imread(convertString(fileName));
		Mat rgb;
		// downsample and use it for processing
		pyrDown(large, rgb);
		Mat smal;
		cvtColor(rgb, smal, CV_BGR2GRAY);
		// morphological gradient
		Mat grad;
		Mat morphKernel = getStructuringElement(MORPH_ELLIPSE, cv::Size(3, 3));
		morphologyEx(smal, grad, MORPH_GRADIENT, morphKernel);
		// binarize
		Mat bw;
		threshold(grad, bw, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);
		// connect horizontally oriented regions
		Mat connected;
		morphKernel = getStructuringElement(MORPH_RECT, cv::Size(9, 1));
		morphologyEx(bw, connected, MORPH_CLOSE, morphKernel);
		// find contours
		Mat mask = Mat::zeros(bw.size(), CV_8UC1);
		vector<vector<cv::Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(connected, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		// filter contours
		for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
		{
			Rect rect = boundingRect(contours[idx]);
			Mat maskROI(mask, rect);
			maskROI = Scalar(0, 0, 0);
			// fill the contour
			drawContours(mask, contours, idx, Scalar(255, 255, 255), CV_FILLED);
			// ratio of non-zero pixels in the filled region
			double r = (double)countNonZero(maskROI) / (rect.width*rect.height);

			if (r > .45 /* assume at least 45% of the area is filled if it contains text */
				&&
				(rect.height > 8 && rect.width > 8) /* constraints on region size */
													/* these two conditions alone are not very robust. better to use something
													like the number of significant peaks in a horizontal projection as a third condition */
				)
			{
				
				if (readFlag) {
					Mat img;
					img = rgb(rect);
					imwrite(convertString(tempCropFileName), img);
					System::Windows::Forms::DialogResult::Yes;
					tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
					api->SetVariable("debug_file","/dev/null");
					if (api->Init(NULL, std::string ("eng").c_str())) {
						fprintf(stderr, "\n");
						//exit(1);
						system("pause");
						fprintf(stderr, "----------------\n");
					}
					char* outText;
					// Open input image with leptonica library
					Pix *image = pixRead(convertString(tempCropFileName).c_str());
					api->SetImage(image);
					// Get OCR result
					outText = api->GetUTF8Text();
					printf("OCR output:\n%s", outText);
					System::String^ const resultStr = gcnew System::String(outText);
					richTextBox->Text += resultStr;
				}
				
					
				rectangle(rgb, rect, Scalar(0, 255, 0), 2);
			}
		}
		//imwrite(OUTPUT_FOLDER_PATH + string("rgb.jpg"), rgb);
		namedWindow("OUTPUT", 1);
		imshow("OUTPUT", rgb);
		//waitKey(0);
	}

#pragma endregion 
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		
		System::Windows::Forms::Form^ form = (gcnew System::Windows::Forms::Form());
		System::Windows::Forms::RichTextBox^ richTextBox = (gcnew System::Windows::Forms::RichTextBox());
		richTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
		form->Controls->Add(richTextBox);
		form->Text = "OCR Results";
		
		readFlag = checkBox1->Checked;
		OpenFileDialog^ choofdlog = gcnew OpenFileDialog();
		choofdlog->Filter = "Image File (*.bmp;*.jpg;*.png;*.gif;*.ico)|*.bmp;*.jpg;*.png;*.gif;*.ico";
		choofdlog->Multiselect = true;

		if (choofdlog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			for each (System::String^ file in choofdlog->FileNames) {
				TextDetect(file, richTextBox);
				if (readFlag) {
					//System::Windows::Forms::DialogResult::Yes;	
					form->ShowDialog();
				}
			}


		}
		
		//TextDetect("C:/Users/90553/Downloads/resim.jpg");
		
	}
	
private: System::Void checkBox1_CheckedChanged_1(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
}



};
}
