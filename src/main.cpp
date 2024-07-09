#include "wx/button.h"
#include "xlsxwriter.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <wx/datetime.h>
#include <wx/listctrl.h>
#include <wx/sysopt.h>
#include <wx/utils.h>
#include <wx/wx.h>

lxw_workbook *workbook = workbook_new("table1.xlsx");
lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "table");
int row = 0;

std::vector<std::string> splitString(const std::string &str,
                                     const std::string &delimiters) {
  std::vector<std::string> result;
  std::string::size_type start = 0;
  std::string::size_type end = str.find_first_of(delimiters);

  while (end != std::string::npos) {
    std::string token = str.substr(start, end - start);
    if (!token.empty()) {
      result.push_back(token);
    }
    start = end + 1;
    end = str.find_first_of(delimiters, start);
  }

  std::string token = str.substr(start);
  if (!token.empty()) {
    result.push_back(token);
  }

  return result;
}

class Frame : public wxFrame {
private:
  wxListCtrl *m_listCtrl;
  long m_itemIndex;
  int m_index = 0;
  wxBoxSizer *m_vbox;
  wxBoxSizer *m_sizer;

public:
  Frame(const wxString &title)
      : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(700, 370)) {
    wxPanel *panel = new wxPanel(this, wxID_ANY);
    m_listCtrl = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, wxLC_REPORT);
    m_sizer = new wxBoxSizer(wxVERTICAL);
    m_vbox = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl *input = new wxTextCtrl(panel, 0, wxEmptyString,
                                       wxDefaultPosition, wxDefaultSize, 0);
    wxButton *button =
        new wxButton(panel, wxID_ANY, wxString::FromUTF8("录入"));
    wxButton *button_del =
        new wxButton(panel, wxID_ANY, wxString::FromUTF8("删除"));
    m_sizer->Add(m_listCtrl, 0, wxALL | wxEXPAND, 5);
    m_sizer->Add(input, 0, wxALL | wxEXPAND, 5);
    m_vbox->Add(button, 0, wxLEFT | wxEXPAND, 5);
    m_vbox->Add(button_del, 0, wxLEFT | wxEXPAND, 0);
    m_sizer->Add(m_vbox, 0, wxALIGN_RIGHT | wxRIGHT, 5);
    button->Bind(wxEVT_BUTTON, &Frame::OnButtonClick, this);
    button_del->Bind(wxEVT_BUTTON, &Frame::OnButtonClickDel, this);
    panel->SetSizer(m_sizer);

    m_listCtrl->InsertColumn(0, wxString::FromUTF8("编号"), wxLIST_FORMAT_LEFT,
                             90);
    m_listCtrl->InsertColumn(1, wxString::FromUTF8("呼号"), wxLIST_FORMAT_LEFT,
                             90);
    m_listCtrl->InsertColumn(2, wxString::FromUTF8("设备"), wxLIST_FORMAT_LEFT,
                             90);
    m_listCtrl->InsertColumn(3, wxString::FromUTF8("天线"), wxLIST_FORMAT_LEFT,
                             90);
    m_listCtrl->InsertColumn(4, wxString::FromUTF8("位置"), wxLIST_FORMAT_LEFT,
                             90);
    m_listCtrl->InsertColumn(5, wxString::FromUTF8("功率"), wxLIST_FORMAT_LEFT,
                             90);
    m_listCtrl->InsertColumn(6, wxString::FromUTF8("时间"), wxLIST_FORMAT_LEFT,
                             140);
    Centre();
  }

  void OnButtonClick(wxCommandEvent &event) {
    wxTextCtrl *input = dynamic_cast<wxTextCtrl *>(FindWindowById(0));
    if (input) {
      wxScopedCharBuffer value = input->GetValue().ToUTF8();
      std::string xlsxdata(value.data());
      std::string str = input->GetValue().ToStdString();
      // std::string str = input->GetValue().ToStdString();
      ;
      if (m_listCtrl == NULL) {
        wxMessageBox("ListCtrl is NULL");
      }
      if (!str.empty() && !xlsxdata.empty()) {

        time_t now = time(0);
        char buffer[80];
        std::tm *tm = std::localtime(&now);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
        std::vector<std::string> datas = splitString(str, " ");
        std::vector<std::string> xlsxdatas = splitString(xlsxdata, " ");

        if (datas.size() < 5) {
          // 填充数据
          int count = 5 - datas.size();
          datas.resize(datas.size() + count, "??");
        }
        if (xlsxdatas.size() < 5) {
          //?
          int count = 5 - xlsxdatas.size();
          xlsxdatas.resize(xlsxdatas.size() + count, "??");
        }

        m_itemIndex =
            m_listCtrl->InsertItem(m_index,
                                   std::to_string(m_index + 1)); // 编号
        m_listCtrl->SetItem(m_itemIndex, 1, datas[0]);           // 呼号
        m_listCtrl->SetItem(m_itemIndex, 2, datas[1]);           // 设备
        m_listCtrl->SetItem(m_itemIndex, 3, datas[2]);           // 天线
        m_listCtrl->SetItem(m_itemIndex, 4, datas[3]);           // 位置
        m_listCtrl->SetItem(m_itemIndex, 5, datas[4]);           // 功率
        m_listCtrl->SetItem(m_itemIndex, 6, buffer);             // 时间
        m_listCtrl->EnsureVisible(m_listCtrl->GetItemCount() - 1);
        // const char *arr[5] = {xlsxdatas[0].c_str(), xlsxdatas[1].c_str(),
        // xlsxdatas[2].c_str(), xlsxdatas[3].c_str(),
        // xlsxdatas[4].c_str()};
        worksheet_write_number(worksheet, row, 0, m_index + 1, NULL);
        worksheet_write_string(worksheet, row, 1, xlsxdatas[0].c_str(), NULL);
        worksheet_write_string(worksheet, row, 2, xlsxdatas[1].c_str(), NULL);
        worksheet_write_string(worksheet, row, 3, xlsxdatas[2].c_str(), NULL);
        worksheet_write_string(worksheet, row, 4, xlsxdatas[3].c_str(), NULL);
        worksheet_write_string(worksheet, row, 5, xlsxdatas[4].c_str(), NULL);
        worksheet_write_string(worksheet, row, 6, buffer, NULL);
        row++;
        m_index++;
      }
    }
  }
  void OnButtonClickDel(wxCommandEvent &event) {
    if (m_itemIndex != wxNOT_FOUND) {
      m_listCtrl->DeleteItem(m_itemIndex);
      m_index--;
      m_itemIndex = wxNOT_FOUND;
    }
  }
};

class App : public wxApp {
public:
  virtual bool OnInit() override {
    // wxSystemOptions::SetOption(wxT("msw.notebook.themed-background"), 1);
    Frame *frame = new Frame(wxString::FromUTF8("102"));
    frame->Show(true);
    return true;
  }
  virtual int OnExit() override {
    workbook_close(workbook);
    return 0;
  }
};

wxIMPLEMENT_APP(App);