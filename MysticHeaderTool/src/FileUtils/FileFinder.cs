using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticHeaderTool.FileUtils
{
    public static class FileFinder
    {
        public static List<FileInfo> GetFilesByModifiedTime(DirectoryInfo directoryToSearch, DateTime time)
        {
            return GetFilesByModifiedTime(directoryToSearch, time, new List<string>());
        }

        public static List<FileInfo> GetFilesByModifiedTime(DirectoryInfo directoryToSearch, DateTime time,
            List<string> excludeFileExtensions)
        {
            var files = new List<FileInfo>();

            foreach (FileInfo file in directoryToSearch.EnumerateFiles())
            {
                if (IsFileValid(file, time, excludeFileExtensions))
                {
                    files.Add(file);
                }
            }

            foreach (DirectoryInfo subDir in directoryToSearch.EnumerateDirectories())
            {
                GetFilesByModifiedTime(directoryToSearch, time, excludeFileExtensions, files);
            }

            return files;
        }

        public static void GetFilesByModifiedTime(DirectoryInfo directoryToSearch, DateTime time,
            List<string> excludeFileExtensions, List<FileInfo> workingList)
        {
            foreach (FileInfo file in directoryToSearch.EnumerateFiles())
            {
                if (IsFileValid(file, time, excludeFileExtensions))
                {
                    workingList.Add(file);
                }
            }

            foreach (DirectoryInfo subDir in directoryToSearch.EnumerateDirectories())
            {
                GetFilesByModifiedTime(directoryToSearch, time, excludeFileExtensions, workingList);
            }
        }

        private static bool IsFileValid(FileInfo file, DateTime startTime, List<string> excludeFileExtensions)
        {
            if (file.LastWriteTime < startTime)
                return false;

            // Could change this in the future to use a HashSet, but then we wouldn't be able to exclude files with 2+ extensions
            // e.g. .generated.h
            foreach (string extension in excludeFileExtensions)
            {
                if (file.Name.EndsWith(extension))
                {
                    return false;
                }
            }

            return true;
        }
    }
}
