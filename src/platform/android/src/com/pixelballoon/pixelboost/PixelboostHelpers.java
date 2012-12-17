package com.pixelballoon.pixelboost;

import android.app.Activity;
import android.media.MediaPlayer;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.content.Context;

import java.io.File;
import java.io.FileDescriptor;
import java.io.IOException;
import java.util.Locale;

public class PixelboostHelpers
{
	public static PixelboostActivity mainActivity;

	public static String getCurrentLocale()
	{
		return Locale.getDefault().getLanguage();
	}

	public static String getUserFilePath()
	{
		Context context = mainActivity.getApplication();
		String filesDir = context.getFilesDir().getAbsolutePath();
		File directory = new File(filesDir + "/");
		directory.mkdirs();
		return filesDir;
	}

    public static FileInfo openFileDescriptor(String filename)
    {
    	Context context = mainActivity.getApplication();
    	AssetManager assetManager = context.getAssets();

    	try
    	{
    		AssetFileDescriptor fileDescriptor = assetManager.openFd(filename);
 
			if (fileDescriptor != null)
			{
				FileInfo info = new FileInfo();
				info.descriptor = fileDescriptor.getFileDescriptor();
				info.offset = fileDescriptor.getStartOffset();
				info.length = fileDescriptor.getLength();

				System.out.println(filename + " " + info.offset + " " + info.length);

				return info;
		    }
		} catch (IOException e) {}
    	
	    return null;
	}

	public static void closeFileDescriptor(FileDescriptor descriptor)
	{

	}

	public static void playMusic(String filename, float volume)
	{
		Context context = mainActivity.getApplication();
    	AssetManager assetManager = context.getAssets();

    	try 
    	{
    		AssetFileDescriptor fileDescriptor = assetManager.openFd(filename);
 
			if (fileDescriptor != null)
			{
				if (mainActivity.music != null)
				{
					stopMusic();
				}

				mainActivity.music = new MediaPlayer();
				mainActivity.music.setDataSource(fileDescriptor.getFileDescriptor(), fileDescriptor.getStartOffset(), fileDescriptor.getLength());
				fileDescriptor.close();
				mainActivity.music.setLooping(true);
				mainActivity.music.prepare();
				mainActivity.music.start();

				setMusicVolume(volume);
			}
		} catch (IOException e) {}
	}

	public static void stopMusic()
	{
		if (mainActivity.music != null)
		{
			mainActivity.music.stop();
			mainActivity.music.release();
			mainActivity.music = null;
		}
	}

	public static void setMusicVolume(float volume)
	{
		if (mainActivity.music != null)
			mainActivity.music.setVolume(volume, volume);
	}

	public static int loadSound(String filename)
	{
		Context context = mainActivity.getApplication();
    	AssetManager assetManager = context.getAssets();

    	try 
    	{
    		AssetFileDescriptor fileDescriptor = assetManager.openFd(filename);
 
			if (fileDescriptor != null)
			{
				int soundId = mainActivity.sounds.load(fileDescriptor.getFileDescriptor(), fileDescriptor.getStartOffset(), fileDescriptor.getLength(), 1);
				fileDescriptor.close();
				return soundId;
			}
    	} catch (IOException e) {}

    	return 0;
	}

	public static int playSound(int soundId)
	{
		return mainActivity.sounds.play(soundId, 1.0f, 1.0f, 0, 0, 1.0f);
	}
}
