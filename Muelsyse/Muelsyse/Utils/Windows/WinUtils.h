#include "mulpch.h"
#include "Muelsyse/Utils/Utils.h"

#include <Shobjidl.h>

namespace mul
{
	std::string wchar2char(const wchar_t* wchar)
	{
		char* m_char;
		int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
		m_char = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
		m_char[len] = '\0';
		std::string str = m_char;
		delete[] m_char;
		return str;
	}

	std::string FileDialogs::openFile(const char* filterDes, const char* filter)
	{
		std::string path;
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog* pFileOpen;

			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

			if (SUCCEEDED(hr))
			{
				DWORD dwFlags;
				hr = pFileOpen->GetOptions(&dwFlags);
				hr = pFileOpen->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);

				COMDLG_FILTERSPEC fileType[] =
				{
					{ L"All files", L"*.*" },
					{ L"Muelsyse Scene (*.flowing)", L"*.flowing" }
				};
				hr = pFileOpen->SetFileTypes(ARRAYSIZE(fileType), fileType);

				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem* pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							path = wchar2char(pszFilePath);
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}

		if(!path.empty())
			return path;
		
		return std::string();
	}

	std::string FileDialogs::saveFile(const char* filterDes, const char* filter)
	{
		std::string path;
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileSaveDialog* pFileSave;

			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
				IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

			if (SUCCEEDED(hr))
			{
				DWORD dwFlags;
				hr = pFileSave->GetOptions(&dwFlags);
				hr = pFileSave->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);

				COMDLG_FILTERSPEC fileType[] =
				{
					{ L"All files", L"*.*" },
					{ L"Muelsyse Scene (*.flowing)", L"*.flowing" }
				};
				hr = pFileSave->SetFileTypes(ARRAYSIZE(fileType), fileType);

				// Show the Open dialog box.
				hr = pFileSave->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem* pItem;
					hr = pFileSave->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							path = wchar2char(pszFilePath);
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileSave->Release();
			}
			CoUninitialize();
		}

		if(!path.empty())
			return path;
		
		return std::string();
	}
}
