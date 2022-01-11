import React from "react"
import { useColorScheme } from 'react-native';

export const useIsDarkMode = () => useColorScheme() === 'dark';