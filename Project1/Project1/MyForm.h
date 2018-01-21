#pragma once
#include <string>
#include <iostream>  
#include <windows.h>


namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			findPorts();
			//
			//TODO: Add the constructor code here
			//

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::IO::Ports::SerialPort^  serialPort1;
	protected:
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  comboBox2;

	private: System::Windows::Forms::Label^  label9;


	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::ComboBox^  comboBox3;






	private: System::ComponentModel::IContainer^  components;

	protected:



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->comboBox3 = (gcnew System::Windows::Forms::ComboBox());
			this->SuspendLayout();
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(198, 71);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 0;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(102, 69);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Scan";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(316, 40);
			this->label1->TabIndex = 2;
			this->label1->Text = L"This is a tool designed for updating data on \r\nesp boards with DDL flash.";
			this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->label2->Location = System::Drawing::Point(13, 69);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(73, 17);
			this->label2->TabIndex = 3;
			this->label2->Text = L"COM Port:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->label3->Location = System::Drawing::Point(13, 107);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(99, 17);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Wi-Fi Settings:";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(92, 136);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(227, 20);
			this->textBox2->TabIndex = 6;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->label4->Location = System::Drawing::Point(12, 136);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(43, 17);
			this->label4->TabIndex = 7;
			this->label4->Text = L"SSID:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->label5->Location = System::Drawing::Point(13, 172);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(73, 17);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Password:";
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(92, 172);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(227, 20);
			this->textBox3->TabIndex = 9;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->label6->Location = System::Drawing::Point(12, 209);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(96, 17);
			this->label6->TabIndex = 10;
			this->label6->Text = L"Time settings:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->label7->Location = System::Drawing::Point(128, 209);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(43, 17);
			this->label7->TabIndex = 12;
			this->label7->Text = L"GMT:";
			// 
			// comboBox2
			// 
			this->comboBox2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(12) {
				L"GMT+1", L"GMT+2", L"GMT+3", L"GMT+4", L"GMT+5",
					L"GMT+6", L"GMT+7", L"GMT+8", L"GMT+9", L"GMT+10", L"GMT+11", L"GMT+12"
			});
			this->comboBox2->Location = System::Drawing::Point(184, 209);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(121, 21);
			this->comboBox2->TabIndex = 13;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->label9->Location = System::Drawing::Point(13, 244);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(110, 17);
			this->label9->TabIndex = 15;
			this->label9->Text = L"NTP Sync Time:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->label10->Location = System::Drawing::Point(13, 276);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(297, 17);
			this->label10->TabIndex = 18;
			this->label10->Text = L"NTP Server (For default settings leave blank):";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(16, 308);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(304, 20);
			this->textBox1->TabIndex = 19;
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(106, 346);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(214, 23);
			this->progressBar1->TabIndex = 20;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(16, 346);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 21;
			this->button2->Text = L"Send Data";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// comboBox3
			// 
			this->comboBox3->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox3->FormattingEnabled = true;
			this->comboBox3->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"00:00", L"12:00, 00:00", L"6:00, 12:00, 18:00, 00:00" });
			this->comboBox3->Location = System::Drawing::Point(131, 244);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(174, 21);
			this->comboBox3->TabIndex = 22;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(340, 387);
			this->Controls->Add(this->comboBox3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->comboBox1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->Text = L"ESP Config";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//find ports
	private: void findPorts(void) {
		this->progressBar1->Value = 0;
		array<String^>^ ObjectArray = SerialPort::GetPortNames();
		this->comboBox1->Items->Clear();
		this->comboBox1->Items->AddRange(ObjectArray);
	}

	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		findPorts();
	}
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		this->progressBar1->Value = 0;
		this->button2->Enabled = false;
		String^ WifiSSID;
		String^ WifiPASSWORD;
		String^ NTPServerAdress;
		String^ GMTTimeString;
		String^ SyncIntervalString;

		Byte GMTTime;

		Byte SyncInterval;
		//0 - 00:00
		//1 - 12:00, 00:00
		//2 - 6:00, 12:00, 18:00, 00:00

		WifiSSID = textBox2->Text;
		WifiPASSWORD = textBox3->Text;
		NTPServerAdress = textBox1->Text;

		if (NTPServerAdress == "") {
			NTPServerAdress = "ntp.time.in.ua";
		}


		GMTTime = comboBox2->SelectedIndex;
		SyncInterval = comboBox3->SelectedIndex;

		GMTTimeString = Convert::ToString(GMTTime);
		SyncIntervalString = Convert::ToString(SyncInterval);

		if (WifiSSID->Length > 32) {
			String^ message = "WiFi SSID is too long.";
			String^ caption = "Error 2";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			System::Windows::Forms::DialogResult result;
			MessageBox::Show(this, message, caption, buttons);
			this->progressBar1->Value = 0;
			this->button2->Enabled = true;
		}
		else if (WifiPASSWORD->Length > 32) {
			String^ message = "WiFi Password is too long.";
			String^ caption = "Error 3";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			System::Windows::Forms::DialogResult result;
			MessageBox::Show(this, message, caption, buttons);
			this->progressBar1->Value = 0;
			this->button2->Enabled = true;
		}
		else if (NTPServerAdress->Length > 100) {
			String^ message = "NTP server adress is too long.";
			String^ caption = "Error 4";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			System::Windows::Forms::DialogResult result;
			MessageBox::Show(this, message, caption, buttons);
			this->progressBar1->Value = 0;
			this->button2->Enabled = true;
		}
		else {
			this->progressBar1->Increment(5);

			//open port
			try {
				this->serialPort1->PortName = this->comboBox1->Text;
				this->serialPort1->BaudRate = Int32::Parse("9600"); 
				this->serialPort1->Open();
			}
			catch (...) {
				//if not opened give an error
				String^ message = "You did not enter a COM port.";
				String^ caption = "Error 1";
				MessageBoxButtons buttons = MessageBoxButtons::OK;
				System::Windows::Forms::DialogResult result;
				MessageBox::Show(this, message, caption, buttons);
				this->progressBar1->Value = 0;
				this->button2->Enabled = true;
			}
			this->progressBar1->Increment(45);
			Sleep(800);
			if (this->serialPort1->IsOpen) {
				this->serialPort1->Write(WifiSSID);	//1
				this->serialPort1->Write(";");
				this->serialPort1->Write(WifiPASSWORD);	//2
				this->serialPort1->Write(";");
				this->serialPort1->Write(GMTTimeString);	//3
				this->serialPort1->Write(";");
				this->serialPort1->Write(SyncIntervalString);	//4
				this->serialPort1->Write(";");
				this->serialPort1->Write(NTPServerAdress);	//5
				this->serialPort1->Write("#");
				this->progressBar1->Increment(50);
				//close port
				this->serialPort1->Close();
				String^ message = "All data sent! Thank you!";
				String^ caption = "Success";
				MessageBoxButtons buttons = MessageBoxButtons::OK;
				System::Windows::Forms::DialogResult result;
				MessageBox::Show(this, message, caption, buttons);
				this->progressBar1->Value = 0;
				this->button2->Enabled = true;
			}

		}


	}
	};
}
