#include "stdafx.h"
#include "VideoDescription.h"
#include "VideoFormatExtractor.h"

VideoDescription::VideoDescription(const web::json::value &input)
{
	_id = input.get(L"id").as_string();
	_title = input.get(L"title").as_string();
	_uploader = input.get(L"uploader").as_string();
	_uploaded = input.get(L"uploaded").as_string();
	_viewCount = input.get(L"viewCount").to_string();
	_duration = input.get(L"duration").as_integer();

	if (input.get(L"thumbnail").has_field(L"hqDefault"))
		_thumbnailUri = input.get(L"thumbnail").get(L"hqDefault").as_string();
	else if (input.get(L"thumbnail").has_field(L"sqDefault"))
		_thumbnailUri = input.get(L"thumbnail").get(L"sqDefault").as_string();

	//-1 signals that no download is currently active for this video
	_downloadPercent = -1;
}

std::wstring VideoDescription::getPath() const
{
	std::wstring filePath;

	auto extensions = VideoFormatExtractor::instance().getAvailableExtensions();
	boost::filesystem::path cachePath(PluginProperties::instance().getProperty(L"cachePath"));
	boost::filesystem::directory_iterator end;

	for (boost::filesystem::directory_iterator iter(cachePath); iter != end; ++iter)
	{
		std::wstring filename = iter->path().filename().wstring();

		if (!boost::filesystem::is_directory(iter->path()))
		{
			for (const auto &extension : extensions)
			{
				if (filename == _id + L"." + extension)
				{
					filePath = iter->path().wstring();
					break;
				}
			}
		}

		if (!filePath.empty())
			break;
	}
	
	return filePath;
}

