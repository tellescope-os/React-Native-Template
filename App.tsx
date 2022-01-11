import React from 'react';

import { TELLESCOPE_TEST_URL } from "@env"

import {
  WithEnduserSession,
  EnduserProvider,
} from "@tellescope/react-components"

import {
  AppNavigator,
  NavigationContainer,
} from "./Routing"

const sessionOptions = {
  host: __DEV__ ? TELLESCOPE_TEST_URL : undefined,
}
console.log('sessionOptions', sessionOptions)

const App = () => (
  <WithEnduserSession sessionOptions={sessionOptions}>
  <EnduserProvider>
  <NavigationContainer>
    <AppNavigator/>
  </NavigationContainer>
  </EnduserProvider>
  </WithEnduserSession>
)

export default App;
