# ScanLocalFiles
Code to scan and catalog point cloud data files.

This code is part of an effort to locate and catalog all Forest Service lidar data that is not held in other data federal collections 
(USGS, NOAA). Some of the data may be available from "local" collections such as states, counties, and universities but these data are
not under federal control.

The first step in this process is to simply locate target file types on our Forest Service corporate storage (T: drive). Once folders 
containing point data are known, a more in-depth scan can take place to collect and aggregate metadata. This will include production of
spatial indexes in GIS-ready formats to help determine if the same data are also held in other federal repositories. In many cases, file
or folder naming may provide clues to other sources of data. Simply put, it takes effort to rename the 1000s of files common with
lidar acquisitions. A key part of the process to detect duplicate data (data held in more than one location or server), is the use of 
file metadata. Namely, bounding boxes, collection date information, and GPS time for a sample of individual points. USGS maintains the
work unit extent spatial metadata (WESM)[https://www.usgs.gov/ngp-standards-and-specifications/wesm-data-dictionary] dictionary for their data. While this is project (or work unit) level metadata, it does contain
information that can help match data held in Forest Service storage to project available from USGS-3DEP. USGS also produces a tile-level
index (TESM) but I have found this to have errors and incomplete data (~2022-2024). The TESM index may be better now.

# Programs
Several programs have been developed to catalog point cloud data. I have an R package (USGSlidar) that facilitates USGS data discovery
and use. I also have an R package that focuses on creating spatial indexes for local and remote file collections (lidarIndexR). This 
package could be used to scan large volumes of data but it isn't necessarily fast. I also have Python code that can scan local and remote
point data files and build spatial indexes. The perceived downside with both R and Python is that they need to list all files in a folder
to determine if there are any point cloud files in the folder. The C++ program, ScanLocalFiles, uses functions that look at each filename
in a folder so they can more quickly determine if a folder has any target file types. However, this same program also recursively walks
folder structures and i don't know how the functions used to do this work. They may have to touch every file to determine if it is a directory
to enable recursion. If this is the case, tghe compiled C++ code may not offer a huge speed gain.

## ScanLocalFiles
This is a C++17 program that recursively scans folders on drives (not remote hosts) looking for point cloud files. Target file types are:
*	LAS
*	LAZ
*	LDA (FUSION format…pre-LAS/LAZ)
*	COPC
*	COPC.LAZ
*	Ept.json

All file types except Entwine format are identified by file extensions. For Entwine data, the ept.json file exists in the "root" location
and the shattered point files (LAZ) exist in an "ept-data" folder. I don't expect to find much data that has been converted to Entwine that 
isn't also available as LAZ or LAS. I included FUSION's LDA format because many of our early (pre ~2005) data may have been delivered as 
ASCII and converted using FUSION. The LAS format was published ~2005 but not adopted by vendors and users for another year or two.

For intial scans, I am not trying to detect ASCII format data. This would be somewhat problematic since data were delivered using different file
extensions and, in some cases, proprietary formats that required special programs to extract the point data. For example, early data from 
Spencer Gross in Oregon was delivered in binary form with a program that could extract various returns from the files and save as ASCII text.
